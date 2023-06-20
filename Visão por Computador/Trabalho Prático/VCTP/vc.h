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
//                           STRUCTS
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//Strcut de imagem
typedef struct {
    unsigned char* data;
    int width, height;
    int channels;			// Bin�rio/Cinzentos=1; RGB=3
    int levels;				// Bin�rio=1; Cinzentos [1,255]; RGB [1,255]
    int bytesperline;		// width * channels
} IVC;

//Struct de propriedades de sinal
typedef struct {
    char nomeCor[20]; //Nome da cor do sinal
	char nomeSinal[50];//Nome do Sinal
} SINAL;

//Struct de propriedades de blob
typedef struct {
	int label;      // Etiqueta do objeto
	int x;          // Coordenada x do canto superior esquerdo da caixa delimitadora
	int y;          // Coordenada y do canto superior esquerdo da caixa delimitadora
	int width;      // Largura da caixa delimitadora
	int height;     // Altura da caixa delimitadora
	int area;       // Área do objeto
	int perimeter;  // Perímetro do objeto
	int xc;         // Coordenada x do centro de gravidade
	int yc;         // Coordenada y do centro de gravidade
} OVC;



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

//Segmentação
int vc_hsv_segmentation(IVC* src, IVC* dst1, IVC* dst2, int hmin1, int hmax1, int smin1, int smax1, int vmin1, int vmax1, int hmin2, int hmax2, int smin2, int smax2, int vmin2, int vmax2);

//HSV -> Grayscale
int vc_hsv_to_grayscale(IVC* src, IVC* dst);

//Grayscale -> HSV
int vc_grayscale_to_hsv(IVC* src, IVC* dst);

//Gray to RGB
int vc_scale_gray_to_red(IVC *src, IVC *dst);

//Contagem de número de píxeis a branco
int vc_count_pixels(IVC *src);

//Threshold manual
int vc_gray_to_binary(IVC *src, IVC *dst, int threshold);

//Threshold média
int vc_gray_to_binary_global_mean(IVC *src, IVC *dst);

//Threshold por vizinhaça do píxel
int vc_gray_to_binary_kernel_8(IVC *src, IVC *dst);

//Etiquetagem de blobs
OVC* vc_binary_blob_labelling(IVC* src, IVC* dst, int* nlabels);

//Obter propriedades do blob
int vc_get_blob_properties(IVC* src, OVC* blobs, int* nlabels);

//Desenhar bounding box
int vc_draw_boundingbox(IVC* srcdst, OVC* blob);

//Desenhar centro de gravidade
int vc_draw_centerofgravity(IVC* srcdst, OVC* blob);

//Dilatação
int vc_binary_dilate(IVC* src, IVC* dst, int kernel);

//Erosão
int vc_binary_erode(IVC* src, IVC* dst, int kernel);

//Abertura
int vc_binary_open(IVC* src, IVC* dst, IVC* aux, int kernel);

//Fecho
int vc_binary_close(IVC* src, IVC* dst, IVC* aux, int kernel);

//imagem binária para tons de cinza
int vc_binary_to_gray(IVC* src, IVC* dst);

//Inverter binária
int vc_binary_invert(IVC* src, IVC* dst);