//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//           INSTITUTO POLIT�CNICO DO C�VADO E DO AVE
//                          2022/2023
//             ENGENHARIA DE SISTEMAS INFORM�TICOS
//                    VIS�O POR COMPUTADOR
//
//             [  BRUNO OLIVEIRA - boliveira@ipca.pt  ]
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


#define VC_DEBUG


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                   ESTRUTURA DE UMA IMAGEM
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


typedef struct {
    unsigned char* data;
    int width, height;
    int channels;			// Bin�rio/Cinzentos=1; RGB=3
    int levels;				// Bin�rio=1; Cinzentos [1,255]; RGB [1,255]
    int bytesperline;		// width * channels
} IVC;


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                    PROT�TIPOS DE FUN��ES
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// FUN��ES: ALOCAR E LIBERTAR UMA IMAGEM
IVC* vc_image_new(int width, int height, int channels, int levels);
IVC* vc_image_free(IVC* image);

// FUN��ES: LEITURA E ESCRITA DE IMAGENS (PBM, PGM E PPM)
IVC* vc_read_image(char* filename);
int vc_write_image(char* filename, IVC* image);

// Gerar negativo da imagem Gray
int vc_gray_negative(IVC* srcdst);

//Gerar negativo de imagem RGB
int vc_rgb_negative(IVC* srcdst);

//Extrair RGB
int vc_rgb_get_red(IVC* srcdst);
int vc_rgb_get_green(IVC* srcdst);
int vc_rgb_get_blue(IVC* srcdst);

//Extrair RGB e mudar para cinza
int vc_rgb_get_red_gray(IVC* srcdst);
int vc_rgb_get_green_gray(IVC* srcdst);
int vc_rgb_get_blue_gray(IVC* srcdst);

//RGB to Gray
int vc_rgb_to_gray(IVC* src, IVC* dst);

//RGB to HSV
int vc_rgb_to_hsv(IVC* src, IVC* dst);

// Calcula o maior de 3 valores
float max(float r, float g, float b);
// Calcula o menor de 3 valores
float min(float r, float g, float b);

//Segmentação
int vc_hsv_segmentation(IVC* src, IVC* dst, int hmin, int hmax, int smin, int smax, int vmin, int vmax);

//Gray to RGB
int vc_scale_gray_to_rgb(IVC *src, IVC *dst);
float max(float a, float b, float c);
float min(float a, float b, float c);

//
int vc_invert_colors(IVC *src, IVC *dst);
int vc_brain_black_area(IVC *src, IVC *dst);
int vc_count_pixels(IVC *src);

int vc_gray_to_binary(IVC *src, IVC *dst, int threshold);
int vc_gray_to_binary_global_mean(IVC *src, IVC *dst);
int vc_gray_to_binary_kernel_8(IVC *src, IVC *dst);
int vc_gray_to_binary_ad(IVC *src, IVC *dst, int t);

//Histograma de uma imagem cinzenta
int vc_gray_histogram_show(IVC *src, IVC *dst);