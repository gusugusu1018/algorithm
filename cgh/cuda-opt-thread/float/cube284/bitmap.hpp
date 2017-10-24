class Bitmap {
   private:
#pragma pack(push,1)
      typedef struct tagBITMAPFILEHEADER
      {
         unsigned short bfType;
         uint32_t  bfSize;
         unsigned short bfReserved1;
         unsigned short bfReserved2;
         uint32_t  bf0ffBits;
      }BITMAPFILEHEADER;
#pragma pack(pop)

      typedef struct tagBITMAPINFOHEADER
      {
         uint32_t  biSize;
         int32_t	biWidth;
         int32_t	biHeight;
         unsigned short  biPlanes;
         unsigned short  biBitCount;
         uint32_t   biCompression;
         uint32_t   biSizeImage;
         int32_t	 biXPelsPerMeter;
         int32_t	 biYPelsPerMeter;
         uint32_t   biCirUsed;
         uint32_t   biCirImportant;
      }BITMAPINFOHEADER;

      typedef struct tagRGBQUAD
      {
         unsigned char  rgbBlue;
         unsigned char  rgbGreen;
         unsigned char  rgbRed;
         unsigned char  rgbReserved;
      }RGBQUAD;

      typedef struct tagBITMAPINFO
      {
         BITMAPINFOHEADER bmiHeader;
         RGBQUAD          bmiColors[1];
      }BITMAPINFO;
      int _w,_h;
      BITMAPFILEHEADER BmpFileHeader;
      BITMAPINFOHEADER BmpInfoHeader;
      RGBQUAD  RGBQuad[256];

      void headerWrite(FILE *_fp) {
         fwrite(&BmpFileHeader, sizeof(BmpFileHeader), 1,_fp);
         fwrite(&BmpInfoHeader, sizeof(BmpInfoHeader) , 1 ,_fp);
         fwrite(&RGBQuad[0], sizeof(RGBQuad[0]) , 256 ,_fp);
      }
   public :
      Bitmap (int _width, int _height) {
         _w = _width;
         _h = _height;
         BmpFileHeader.bfType                =19778;
         BmpFileHeader.bfSize                =14+40+1024+(_w*_h);
         BmpFileHeader.bfReserved1           =0;
         BmpFileHeader.bfReserved2           =0;
         BmpFileHeader.bf0ffBits             =14+40+1024;
         BmpInfoHeader.biSize                =40;
         BmpInfoHeader.biWidth               =_w;
         BmpInfoHeader.biHeight              =_h;
         BmpInfoHeader.biPlanes              =1;
         BmpInfoHeader.biBitCount            =8;     //256階調
         BmpInfoHeader.biCompression         =0L;
         BmpInfoHeader.biSizeImage           =0L;
         BmpInfoHeader.biXPelsPerMeter       =0L;
         BmpInfoHeader.biYPelsPerMeter       =0L;
         BmpInfoHeader.biCirUsed             =0L;
         BmpInfoHeader.biCirImportant        =0L;
         for(int i=0;i<256;i++){
            RGBQuad[i].rgbBlue                =i;
            RGBQuad[i].rgbGreen               =i;
            RGBQuad[i].rgbRed                 =i;
            RGBQuad[i].rgbReserved            =0;
         }
      }
      void write(unsigned char *_img, FILE *_fp) {
         headerWrite(_fp);
         fwrite(_img,sizeof(unsigned char),_w*_h,_fp);
      }
};
