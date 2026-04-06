#include <png.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>
#include <sys/stat.h>	  // mkdir

// RAII wrapper around a PNG file read
struct PngImage {
	int width  = 0;
	int height = 0;
	// Packed RGB (3 bytes per pixel), row-major
	std::vector<uint8_t> pixels;
};

std::pair<int, int> reindex[] = {
	{0, 0},	  {0, 1},	{0, 2},	  {0, 3},	{1, 0},	  {1, 1},	{1, 2},	  {1, 3},	{0, 4}, {0, 5}, {0, 7},	 {0, 11},
	{1, 4},	  {1, 5},	{1, 7},	  {1, 11},	{0, 6},	  {0, 9},	{0, 10},  {0, 8},	{1, 6}, {1, 9}, {1, 10}, {1, 8},

	{-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1},

	{2, 0},	  {2, 1},	{2, 2},	  {2, 3},	{3, 0},	  {3, 1},	{3, 2},	  {3, 3},	{2, 4}, {2, 5}, {2, 7},	 {2, 11},
	{3, 4},	  {3, 5},	{3, 7},	  {3, 11},	{2, 6},	  {2, 9},	{2, 10},  {2, 8},	{3, 6}, {3, 9}, {3, 10}, {3, 8},

	{-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1},
};

PngImage load_png(const char *path) {
	FILE *fp = fopen(path, "rb");
	if (!fp) throw std::runtime_error(std::string("Cannot open file: ") + path);

	// Verify PNG signature
	uint8_t sig[8];
	if (fread(sig, 1, 8, fp) != 8 || png_sig_cmp(sig, 0, 8)) {
		fclose(fp);
		throw std::runtime_error("Not a valid PNG file.");
	}

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if (!png_ptr) {
		fclose(fp);
		throw std::runtime_error("png_create_read_struct failed");
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, nullptr, nullptr);
		fclose(fp);
		throw std::runtime_error("png_create_info_struct failed");
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
		fclose(fp);
		throw std::runtime_error("libpng error while reading.");
	}

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);	   // we already consumed the signature
	png_read_info(png_ptr, info_ptr);

	int		 width	= (int)png_get_image_width(png_ptr, info_ptr);
	int		 height = (int)png_get_image_height(png_ptr, info_ptr);
	png_byte color	= png_get_color_type(png_ptr, info_ptr);
	png_byte depth	= png_get_bit_depth(png_ptr, info_ptr);

	// Normalise to 8-bit RGB
	if (depth == 16) png_set_strip_16(png_ptr);
	if (color == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png_ptr);
	if (color == PNG_COLOR_TYPE_GRAY || color == PNG_COLOR_TYPE_GRAY_ALPHA) png_set_gray_to_rgb(png_ptr);
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png_ptr);
	if (color & PNG_COLOR_MASK_ALPHA) png_set_strip_alpha(png_ptr);
	png_read_update_info(png_ptr, info_ptr);

	int					   rowbytes = (int)png_get_rowbytes(png_ptr, info_ptr);
	std::vector<uint8_t>   pixels((size_t)height * rowbytes);
	std::vector<png_bytep> rows((size_t)height);
	for (int y = 0; y < height; ++y)
		rows[y] = pixels.data() + (size_t)y * rowbytes;

	png_read_image(png_ptr, rows.data());
	png_read_end(png_ptr, nullptr);
	png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
	fclose(fp);

	PngImage img;
	img.width  = width;
	img.height = height;
	img.pixels = std::move(pixels);
	return img;
}

// ---------------------------------------------------------------------------
// Save a packed-RGB buffer as an 8-bit RGB PNG file.
// ---------------------------------------------------------------------------
static void save_png(const char *path, const uint8_t *pixels, int width, int height) {
	FILE *fp = fopen(path, "wb");
	if (!fp) throw std::runtime_error(std::string("Cannot open for writing: ") + path);

	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if (!png_ptr) {
		fclose(fp);
		throw std::runtime_error("png_create_write_struct failed");
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_write_struct(&png_ptr, nullptr);
		fclose(fp);
		throw std::runtime_error("png_create_info_struct failed");
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(fp);
		throw std::runtime_error(std::string("libpng write error: ") + path);
	}

	png_init_io(png_ptr, fp);
	png_set_IHDR(png_ptr, info_ptr, (png_uint_32)width, (png_uint_32)height,
				 8,		// bit depth
				 PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png_ptr, info_ptr);

	// Write row by row (pixels are already tightly packed RGB)
	for (int y = 0; y < height; ++y) {
		const uint8_t *row = pixels + (size_t)y * width * 3;
		png_write_row(png_ptr, const_cast<png_bytep>(row));
	}

	png_write_end(png_ptr, nullptr);
	png_destroy_write_struct(&png_ptr, &info_ptr);
	fclose(fp);
}

static void print_usage(const char *prog) {
	fprintf(stderr,
			"Usage:\n"
			"  %s --single <input.png> <output.bin>      "
			"Save all 64 tiles into one binary file\n"
			"  %s --split  <input.png> <output_dir>      "
			"Save each tile as a separate PNG in a subfolder\n",
			prog, prog);
}

// ---------------------------------------------------------------------------
// Extract the tile at grid position (row, col) into tile_buf.
// ---------------------------------------------------------------------------
static void extract_tile(const PngImage &img, int N, int cell_size, int tile_size, int row, int col,
						 std::vector<uint8_t> &tile_buf) {
	const int origin_x = col * cell_size;
	const int origin_y = row * cell_size;

	for (int ty = 0; ty < tile_size; ++ty) {
		int src_y = origin_y + ty;
		if (src_y >= N) src_y = N - 1;

		for (int tx = 0; tx < tile_size; ++tx) {
			int src_x = origin_x + tx;
			if (src_x >= N) src_x = N - 1;

			const uint8_t *src = img.pixels.data() + ((size_t)src_y * N + src_x) * 3;
			uint8_t		  *dst = tile_buf.data() + ((size_t)ty * tile_size + tx) * 3;
			dst[0]			   = src[0];	 // R
			dst[1]			   = src[1];	 // G
			dst[2]			   = src[2];	 // B
		}
	}
}

// ---------------------------------------------------------------------------
// Write helper – aborts on error.
// ---------------------------------------------------------------------------
static void write_tile(FILE *fp, const std::vector<uint8_t> &buf, const char *path) {
	if (fwrite(buf.data(), 1, buf.size(), fp) != buf.size()) {
		fprintf(stderr, "Write error on: %s\n", path);
		fclose(fp);
		exit(1);
	}
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------
int main(int argc, char *argv[]) {
	if (argc < 4) {
		print_usage(argv[0]);
		return 1;
	}

	const char *mode_flag  = argv[1];
	const char *input_path = argv[2];
	const char *output_arg = argv[3];

	bool mode_single = false;

	if (strcmp(mode_flag, "--single") == 0) mode_single = true;
	else if (strcmp(mode_flag, "--split") != 0) {
		fprintf(stderr, "Unknown flag: %s\n", mode_flag);
		print_usage(argv[0]);
		return 1;
	}

	// Load source image
	PngImage img;
	try {
		img = load_png(input_path);
	} catch (const std::exception &e) {
		fprintf(stderr, "Error loading PNG: %s\n", e.what());
		return 1;
	}

	if (img.width != img.height) {
		fprintf(stderr, "Image is not square (%dx%d).\n", img.width, img.height);
		return 1;
	}

	const int N			= img.width;
	const int cell_size = N / 8;
	const int tile_size = (int)std::round(0.9 * cell_size);

	if (tile_size <= 0 || cell_size <= 0) {
		fprintf(stderr, "N=%d is too small to produce valid tiles.\n", N);
		return 1;
	}

	printf("N=%d  cell_size=%d  tile_size=%d\n", N, cell_size, tile_size);

	std::vector<uint8_t> tile_buf((size_t)tile_size * tile_size * 3);

	// -----------------------------------------------------------------------
	// Mode A: single binary file
	// -----------------------------------------------------------------------
	if (mode_single) {
		FILE *out = fopen(output_arg, "wb");
		if (!out) {
			fprintf(stderr, "Cannot open output file: %s\n", output_arg);
			return 1;
		}

		for (int row = 0; row < 8; ++row) {
			for (int col = 0; col < 8; ++col) {
				auto remapped_id = reindex[row * 8 + col];
				if (remapped_id.first == -1 || remapped_id.second == -1) { continue; }

				extract_tile(img, N, cell_size, tile_size, row, col, tile_buf);
				write_tile(out, tile_buf, output_arg);
			}
		}

		fclose(out);
		size_t total = (size_t)64 * tile_size * tile_size * 3;
		printf("Wrote 64 tiles (%dx%d px each) → %zu bytes → %s\n", tile_size, tile_size, total, output_arg);
	}

	// -----------------------------------------------------------------------
	// Mode B: 64 individual PNG files inside a subfolder
	//   Naming: tile_00.png … tile_63.png  (row-major, zero-padded)
	// -----------------------------------------------------------------------
	else {
		// Create the output directory (ignore EEXIST)
#ifdef _WIN32
		int rc = _mkdir(output_arg);
#else
		int rc = mkdir(output_arg, 0755);
#endif
		if (rc != 0 && errno != EEXIST) {
			perror("mkdir");
			return 1;
		}

		int tile_index = 0;
		for (int row = 0; row < 8; ++row) {
			for (int col = 0; col < 8; ++col) {
				auto remapped_id = reindex[row * 8 + col];
				if (remapped_id.first == -1 || remapped_id.second == -1) { continue; }
				tile_index = remapped_id.first * 12 + remapped_id.second;

				extract_tile(img, N, cell_size, tile_size, row, col, tile_buf);

				char path[1024];
				snprintf(path, sizeof(path), "%s/tile_%02d.png", output_arg, tile_index);

				try {
					save_png(path, tile_buf.data(), tile_size, tile_size);
				} catch (const std::exception &e) {
					fprintf(stderr, "Error: %s\n", e.what());
					return 1;
				}
			}
		}

		printf("Wrote 64 PNG files (%dx%d px each) → %s/\n", tile_size, tile_size, output_arg);
	}

	return 0;
}
