using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Drawing.Imaging;

namespace OSSP_lab2
{
    public class MyImage
    {
        public MyImage(int rows, int columns)
        {
            _image = new Bitmap(rows, columns);
        }

        public void adjustBrightness(int nBright)
        {
            BitmapData bmData = _image.LockBits(new Rectangle(0, 0, _image.Width, _image.Height), 
            ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb); 
            int stride = bmData.Stride; 
            System.IntPtr Scan0 = bmData.Scan0; 
            unsafe 
            { 
                byte * p = (byte *)(void *)Scan0;
                int nOffset = stride - _image.Width * 3;
                int nWidth = _image.Width * 3;
                for (int y = 0; y < _image.Height; ++y)
                {
                    for(int x=0; x < nWidth; ++x )
                    {
                        int nVal = (int)(p[0] + nBright);

                        if (nVal < 0) nVal = 0;
                        if (nVal > 255) nVal = 255;

                        p[0] = (byte) nVal;

                        ++p;
                    }
                    p += nOffset;
                }
            }

            _image.UnlockBits(bmData);
        }

        public void adjustContrast(double fCont)
        {
            BitmapData bmData = _image.LockBits(new Rectangle(0, 0, _image.Width, _image.Height),
            ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);
            int stride = bmData.Stride;
            System.IntPtr Scan0 = bmData.Scan0;
            unsafe
            {
                byte* p = (byte*)(void*)Scan0;
                int nOffset = stride - _image.Width * 3;
                int nWidth = _image.Width * 3;
                for (int y = 0; y < _image.Height; ++y)
                {
                    for (int x = 0; x < nWidth; ++x)
                    {
                        double pixel = p[0] / 255.0;
                        pixel -= 0.5;
                        pixel *= fCont;
                        pixel += 0.5;
                        pixel *= 255;
                        if (pixel < 0) pixel = 0;
                        if (pixel > 255) pixel = 255;
                        p[0] = (byte)pixel;

                        ++p;
                    }
                    p += nOffset;
                }
            }

            _image.UnlockBits(bmData);
        }

        public void resize(int width, int height)
        {
            Bitmap image_old = (Bitmap) _image.Clone();
            _image = new Bitmap(width, height, image_old.PixelFormat);

            double nXFactor = (double) image_old.Width / (double) width;
            double nYFactor = (double) image_old.Height / (double) height;

            double fraction_x, fraction_y, one_minus_x, one_minus_y;
            int ceil_x, ceil_y, floor_x, floor_y;
            Color c1 = new Color();
            Color c2 = new Color();
            Color c3 = new Color();
            Color c4 = new Color();
            byte red, green, blue;

            byte b1, b2;

            for (int x = 0; x < _image.Width; ++x)
                for (int y = 0; y < _image.Height; ++y)
                {
                    // Setup
                    floor_x = (int)Math.Floor(x * nXFactor);
                    floor_y = (int)Math.Floor(y * nYFactor);
                    ceil_x = floor_x + 1;
                    if (ceil_x >= image_old.Width) ceil_x = floor_x;
                    ceil_y = floor_y + 1;
                    if (ceil_y >= image_old.Height) ceil_y = floor_y;
                    fraction_x = x * nXFactor - floor_x;
                    fraction_y = y * nYFactor - floor_y;
                    one_minus_x = 1.0 - fraction_x;
                    one_minus_y = 1.0 - fraction_y;

                    c1 = image_old.GetPixel(floor_x, floor_y);
                    c2 = image_old.GetPixel(ceil_x, floor_y);
                    c3 = image_old.GetPixel(floor_x, ceil_y);
                    c4 = image_old.GetPixel(ceil_x, ceil_y);

                    // Blue
                    b1 = (byte)(one_minus_x * c1.B + fraction_x * c2.B);

                    b2 = (byte)(one_minus_x * c3.B + fraction_x * c4.B);

                    blue = (byte)(one_minus_y * (double)(b1) + fraction_y * (double)(b2));

                    // Green
                    b1 = (byte)(one_minus_x * c1.G + fraction_x * c2.G);

                    b2 = (byte)(one_minus_x * c3.G + fraction_x * c4.G);

                    green = (byte)(one_minus_y * (double)(b1) + fraction_y * (double)(b2));

                    // Red
                    b1 = (byte)(one_minus_x * c1.R + fraction_x * c2.R);

                    b2 = (byte)(one_minus_x * c3.R + fraction_x * c4.R);

                    red = (byte)(one_minus_y * (double)(b1) + fraction_y * (double)(b2));

                    _image.SetPixel(x, y, System.Drawing.Color.FromArgb(255, red, green, blue));
                }
        }

        public Bitmap image { get { return _image; } set { _image = value;} }
        private Bitmap _image;
    }
}
