// Include the Ruby headers and goodies
#include "ruby.h"
#include "stdlib.h"
#include "stdio.h"
#include "time.h"

void apply_primitive(double* primitive, double total, int m, int *image_data, int *out_image_data, int x, int y, int height, int width);
void reset_effective_primitive(double *effective_primitive, double *primitive, int m, int x, int y, int height, int width);
char is_out_of_bound(int x, int y, int height, int width);
double *convert_ruby_array_double(VALUE rb_array);
int *convert_ruby_array_img_data(VALUE rb_array);

VALUE do_gauss_method(VALUE self, VALUE rb_m, VALUE rb_primitive, VALUE rb_image_data, VALUE rb_height, VALUE rb_width)
{
  int image_length = RARRAY_LEN(rb_image_data);

  double *primitive = convert_ruby_array_double(rb_primitive);
  int *image_data = convert_ruby_array_img_data(rb_image_data);
  int *out_image_data = malloc(sizeof(int) * image_length * 3);
  int m = NUM2INT(rb_m);
  int height = NUM2INT(rb_height);
  int width = NUM2INT(rb_width);

  double total = 0.0;
  for (int offset = 0; offset < (2 * m + 1) * (2 * m + 1); offset++)
    total += primitive[offset];

  if (getenv("DEBUG") != NULL) {
    fprintf(stderr, "M: %d (2m + 1 = %d)\n", m, 2 * m + 1);
  }
  time_t t1 = time(NULL);
  for (int x = 0; x < height; x++)
    for (int y = 0; y < width; y++) {
      apply_primitive(primitive, total, m, image_data, out_image_data, x, y, height, width);
    }
  time_t t2 = time(NULL);
  if (getenv("DEBUG") != NULL) {
    fprintf(stderr, "Elasped: %ld s\n", t2 - t1);
  }

  VALUE res = rb_ary_new_capa(image_length);
  VALUE pixel;
  for (int i = 0; i < image_length; i++) {
    pixel = rb_ary_new_capa(3);
    rb_ary_store(pixel, 0, INT2NUM(out_image_data[3 * i]));
    rb_ary_store(pixel, 1, INT2NUM(out_image_data[3 * i + 1]));
    rb_ary_store(pixel, 2, INT2NUM(out_image_data[3 * i + 2]));
    rb_ary_store(res, i, pixel);
  }

  free(primitive);
  free(image_data);
  free(out_image_data);
  return res;
}

void apply_primitive(double* primitive, double total, int m, int *image_data, int *out_image_data, int x, int y, int height, int width)
{
  /* reset_effective_primitive(effective_primitive, primitive, m, x, y, height, width); */

  double sums[3] = {0.0, 0.0, 0.0};
  int current[3];
  for (int offset_x = -m; offset_x <= m; offset_x++)
    for (int offset_y = -m; offset_y <= m; offset_y++)
    {
      int nx = x + offset_x;
      int ny = y + offset_y;

      if (nx < 0)
        nx = -nx;
      if (nx >= height)
        nx = 2 * height - nx - 1;
      if (ny < 0)
        ny = -ny;
      if (ny >= width)
        ny = 2 * width - ny - 1;
      int base = (nx * width + ny) * 3;
      current[0] = image_data[base]; current[1] = image_data[base + 1]; current[2] = image_data[base + 2];

      base = (offset_x + m) * (2 * m + 1) + offset_y + m;
      sums[0] += primitive[base] * current[0];
      sums[1] += primitive[base] * current[1];
      sums[2] += primitive[base] * current[2];
    }

  sums[0] /= total;
  sums[1] /= total;
  sums[2] /= total;

  int base = (x * width + y) * 3;
  out_image_data[base] = (int) (sums[0] + 0.5);
  out_image_data[base + 1] = (int) (sums[1] + 0.5);
  out_image_data[base + 2] = (int) (sums[2] + 0.5);
}

void reset_effective_primitive(double *effective_primitive, double *primitive, int m, int x, int y, int height, int width)
{
  for (int offset = 0; offset < (2 * m + 1) * (2 * m + 1); offset++)
    effective_primitive[offset] = primitive[offset];
  for (int offset_x = -m; offset_x <= m; offset_x++)
    for (int offset_y = -m; offset_y <= m; offset_y++)
    {
      int nx = x + offset_x;
      int ny = y + offset_y;
      if (is_out_of_bound(nx, ny, height, width)) {
        int base = (offset_x + m) * (2 * m + 1) + offset_y + m;
        double value = effective_primitive[base];
        effective_primitive[base] = 0.0;
        int alive_count = 0;
        for (int offset = 0; offset < (2 * m + 1) * (2 * m + 1); offset++)
          if (effective_primitive[offset] != 0.0)
            alive_count += 1;
        double addition = value / alive_count;
        for (int offset = 0; offset < (2 * m + 1) * (2 * m + 1); offset++)
          if (effective_primitive[offset] != 0.0)
            effective_primitive[offset] += addition;
      }
    }
}

char is_out_of_bound(int x, int y, int height, int width)
{
  return x < 0 || x >= height || y < 0 || y >= width;
}

double *convert_ruby_array_double(VALUE rb_array)
{
  int ar_length = RARRAY_LEN(rb_array);
  double *res = malloc(sizeof(double) * ar_length);
  for (int i = 0; i < ar_length; i++) {
    res[i] = NUM2DBL(rb_ary_entry(rb_array, i));
  }
  return res;
}

int *convert_ruby_array_img_data(VALUE rb_array)
{
  int ar_length = RARRAY_LEN(rb_array);
  int *res = malloc(sizeof(int) * ar_length * 3);
  for (int i = 0; i < ar_length; i++) {
    VALUE pixel = rb_ary_entry(rb_array, i);
    res[3 * i] = NUM2INT(rb_ary_entry(pixel, 0));
    res[3 * i + 1] = NUM2INT(rb_ary_entry(pixel, 1));
    res[3 * i + 2] = NUM2INT(rb_ary_entry(pixel, 2));
  }
  return res;
}

// Defining a space for information and references about the module to be stored internally
VALUE Utils = Qnil;
VALUE Processors = Qnil;
VALUE Ext = Qnil;

// The initialization method for this module
void Init_ext() {
  Utils = rb_define_module("Utils");
  Processors = rb_define_module_under(Utils, "Processors");
  Ext = rb_define_module_under(Processors, "GaussianExtModule");
  rb_define_method(Ext, "do_gauss", do_gauss_method, 5);
}
