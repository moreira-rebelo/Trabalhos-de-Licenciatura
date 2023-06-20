//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//           INSTITUTO POLIT�CNICO DO C�VADO E DO AVE
//                          2022/2023
//             ENGENHARIA DE SISTEMAS INFORM�TICOS
//                    VIS�O POR COMPUTADOR
//
//             [  BRUNO OLIVEIRA - boliveira@ipca.pt  ]
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Desabilita (no MSVC++) warnings de fun��es n�o seguras (fopen, sscanf, etc...)
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>
#include "vc.h"


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//            FUN��ES: ALOCAR E LIBERTAR UMA IMAGEM
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


// Alocar mem�ria para uma imagem
IVC* vc_image_new(int width, int height, int channels, int levels)
{
	IVC* image = (IVC*)malloc(sizeof(IVC));

	if (image == NULL) return NULL;
	if ((levels <= 0) || (levels > 255)) return NULL;

	image->width = width;
	image->height = height;
	image->channels = channels;
	image->levels = levels;
	image->bytesperline = image->width * image->channels;
	image->data = (unsigned char*)malloc(image->width * image->height * image->channels * sizeof(char));

	if (image->data == NULL)
	{
		return vc_image_free(image);
	}

	return image;
}


// Libertar mem�ria de uma imagem
IVC* vc_image_free(IVC* image)
{
	if (image != NULL)
	{
		if (image->data != NULL)
		{
			free(image->data);
			image->data = NULL;
		}

		free(image);
		image = NULL;
	}

	return image;
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//    FUN��ES: LEITURA E ESCRITA DE IMAGENS (PBM, PGM E PPM)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


char* netpbm_get_token(FILE* file, char* tok, int len)
{
	char* t;
	int c;

	for (;;)
	{
		while (isspace(c = getc(file)));
		if (c != '#') break;
		do c = getc(file);
		while ((c != '\n') && (c != EOF));
		if (c == EOF) break;
	}

	t = tok;

	if (c != EOF)
	{
		do
		{
			*t++ = c;
			c = getc(file);
		} while ((!isspace(c)) && (c != '#') && (c != EOF) && (t - tok < len - 1));

		if (c == '#') ungetc(c, file);
	}

	*t = 0;

	return tok;
}


long int unsigned_char_to_bit(unsigned char* datauchar, unsigned char* databit, int width, int height)
{
	int x, y;
	int countbits;
	long int pos, counttotalbytes;
	unsigned char* p = databit;

	*p = 0;
	countbits = 1;
	counttotalbytes = 0;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = width * y + x;

			if (countbits <= 8)
			{
				// Numa imagem PBM:
				// 1 = Preto
				// 0 = Branco
				//*p |= (datauchar[pos] != 0) << (8 - countbits);

				// Na nossa imagem:
				// 1 = Branco
				// 0 = Preto
				*p |= (datauchar[pos] == 0) << (8 - countbits);

				countbits++;
			}
			if ((countbits > 8) || (x == width - 1))
			{
				p++;
				*p = 0;
				countbits = 1;
				counttotalbytes++;
			}
		}
	}

	return counttotalbytes;
}


void bit_to_unsigned_char(unsigned char* databit, unsigned char* datauchar, int width, int height)
{
	int x, y;
	int countbits;
	long int pos;
	unsigned char* p = databit;

	countbits = 1;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = width * y + x;

			if (countbits <= 8)
			{
				// Numa imagem PBM:
				// 1 = Preto
				// 0 = Branco
				//datauchar[pos] = (*p & (1 << (8 - countbits))) ? 1 : 0;

				// Na nossa imagem:
				// 1 = Branco
				// 0 = Preto
				datauchar[pos] = (*p & (1 << (8 - countbits))) ? 0 : 1;

				countbits++;
			}
			if ((countbits > 8) || (x == width - 1))
			{
				p++;
				countbits = 1;
			}
		}
	}
}


IVC* vc_read_image(char* filename)
{
	FILE* file = NULL;
	IVC* image = NULL;
	unsigned char* tmp;
	char tok[20];
	long int size, sizeofbinarydata;
	int width, height, channels;
	int levels = 255;
	int v;

	// Abre o ficheiro
	if ((file = fopen(filename, "rb")) != NULL)
	{
		// Efectua a leitura do header
		netpbm_get_token(file, tok, sizeof(tok));

		if (strcmp(tok, "P4") == 0) { channels = 1; levels = 1; }	// Se PBM (Binary [0,1])
		else if (strcmp(tok, "P5") == 0) channels = 1;				// Se PGM (Gray [0,MAX(level,255)])
		else if (strcmp(tok, "P6") == 0) channels = 3;				// Se PPM (RGB [0,MAX(level,255)])
		else
		{
#ifdef VC_DEBUG
			printf("ERROR -> vc_read_image():\n\tFile is not a valid PBM, PGM or PPM file.\n\tBad magic number!\n");
#endif

			fclose(file);
			return NULL;
		}

		if (levels == 1) // PBM
		{
			if (sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &width) != 1 ||
				sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &height) != 1)
			{
#ifdef VC_DEBUG
				printf("ERROR -> vc_read_image():\n\tFile is not a valid PBM file.\n\tBad size!\n");
#endif

				fclose(file);
				return NULL;
			}

			// Aloca mem�ria para imagem
			image = vc_image_new(width, height, channels, levels);
			if (image == NULL) return NULL;

			sizeofbinarydata = (image->width / 8 + ((image->width % 8) ? 1 : 0)) * image->height;
			tmp = (unsigned char*)malloc(sizeofbinarydata);
			if (tmp == NULL) return 0;

#ifdef VC_DEBUG
			printf("\nchannels=%d w=%d h=%d levels=%d\n", image->channels, image->width, image->height, levels);
#endif

			if ((v = fread(tmp, sizeof(unsigned char), sizeofbinarydata, file)) != sizeofbinarydata)
			{
#ifdef VC_DEBUG
				printf("ERROR -> vc_read_image():\n\tPremature EOF on file.\n");
#endif

				vc_image_free(image);
				fclose(file);
				free(tmp);
				return NULL;
			}

			bit_to_unsigned_char(tmp, image->data, image->width, image->height);

			free(tmp);
		}
		else // PGM ou PPM
		{
			if (sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &width) != 1 ||
				sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &height) != 1 ||
				sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &levels) != 1 || levels <= 0 || levels > 255)
			{
#ifdef VC_DEBUG
				printf("ERROR -> vc_read_image():\n\tFile is not a valid PGM or PPM file.\n\tBad size!\n");
#endif

				fclose(file);
				return NULL;
			}

			// Aloca mem�ria para imagem
			image = vc_image_new(width, height, channels, levels);
			if (image == NULL) return NULL;

#ifdef VC_DEBUG
			printf("\nchannels=%d w=%d h=%d levels=%d\n", image->channels, image->width, image->height, levels);
#endif

			size = image->width * image->height * image->channels;

			if ((v = fread(image->data, sizeof(unsigned char), size, file)) != size)
			{
#ifdef VC_DEBUG
				printf("ERROR -> vc_read_image():\n\tPremature EOF on file.\n");
#endif

				vc_image_free(image);
				fclose(file);
				return NULL;
			}
		}

		fclose(file);
	}
	else
	{
#ifdef VC_DEBUG
		printf("ERROR -> vc_read_image():\n\tFile not found.\n");
#endif
	}

	return image;
}


int vc_write_image(char* filename, IVC* image)
{
	FILE* file = NULL;
	unsigned char* tmp;
	long int totalbytes, sizeofbinarydata;

	if (image == NULL) return 0;

	if ((file = fopen(filename, "wb")) != NULL)
	{
		if (image->levels == 1)
		{
			sizeofbinarydata = (image->width / 8 + ((image->width % 8) ? 1 : 0)) * image->height + 1;
			tmp = (unsigned char*)malloc(sizeofbinarydata);
			if (tmp == NULL) return 0;

			fprintf(file, "%s %d %d\n", "P4", image->width, image->height);

			totalbytes = unsigned_char_to_bit(image->data, tmp, image->width, image->height);
			printf("Total = %ld\n", totalbytes);
			if (fwrite(tmp, sizeof(unsigned char), totalbytes, file) != totalbytes)
			{
#ifdef VC_DEBUG
				fprintf(stderr, "ERROR -> vc_read_image():\n\tError writing PBM, PGM or PPM file.\n");
#endif

				fclose(file);
				free(tmp);
				return 0;
			}

			free(tmp);
		}
		else
		{
			fprintf(file, "%s %d %d 255\n", (image->channels == 1) ? "P5" : "P6", image->width, image->height);

			if (fwrite(image->data, image->bytesperline, image->height, file) != image->height)
			{
#ifdef VC_DEBUG
				fprintf(stderr, "ERROR -> vc_read_image():\n\tError writing PBM, PGM or PPM file.\n");
#endif

				fclose(file);
				return 0;
			}
		}

		fclose(file);

		return 1;
	}

	return 0;
}

// Gerar negativo da imagem Gray
int vc_gray_negative(IVC* srcdst)
{
	unsigned char* data = (unsigned char*)srcdst->data;
	int width = srcdst->width; int height = srcdst->height;
	int bytesperline = srcdst->width * srcdst->channels;
	int channels = srcdst->channels;
	int x, y;
	long int pos;

	// Verifica��o de erros
	if ((srcdst->width <= 0) || (srcdst->height <= 0) || (srcdst->data == NULL)) return 0; if (channels != 1) return 0;

	// Inverte a imagem Gray
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;
			data[pos] = 255 - data[pos];
		}
	}

	return 1;
}

//Gerar negativo de imagem RGB
int vc_rgb_negative(IVC* srcdst)
{
	unsigned char* data = (unsigned char*)srcdst->data;
	int width = srcdst->width; int height = srcdst->height;
	int bytesperline = srcdst->width * srcdst->channels;
	int channels = srcdst->channels;
	int x, y;
	long int pos;

	// Verifica��o de erros
	if ((srcdst->width <= 0) || (srcdst->height <= 0) || (srcdst->data == NULL)) return 0; if (channels != 3) return 0;

	//Inverte a imagem RGB
	for (y = 0; y < width; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;

			data[pos] = 255 - data[pos];
			data[pos + 1] = 255 - data[pos + 1];
			data[pos + 2] = 255 - data[pos + 2];
		}
	}
	return 1;
}

//Extrair RGB
int vc_rgb_get_red(IVC* srcdst)
{
	unsigned char* data = (unsigned char*)srcdst->data;
	int width = srcdst->width;
	int height = srcdst->height;
	int bytesperline = srcdst->width * srcdst->channels;
	int channels = srcdst->channels;
	int x, y;
	long int pos;

	// Verifica��o de erros
	if ((srcdst->width <= 0) || (srcdst->height <= 0) || (srcdst->data == NULL)) return 0; if (channels != 3) return 0;
	if (channels != 3) return 0;

	//Extrair a cor vermelha
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;

			data[pos + 1] = 0;
			data[pos + 2] = 0;
		}
	}
	return 1;
}

int vc_rgb_get_green(IVC* srcdst)
{
	unsigned char* data = (unsigned char*)srcdst->data;
	int width = srcdst->width; int height = srcdst->height;
	int bytesperline = srcdst->width * srcdst->channels;
	int channels = srcdst->channels;
	int x, y;
	long int pos;

	// Verifica��o de erros
	if ((srcdst->width <= 0) || (srcdst->height <= 0) || (srcdst->data == NULL)) return 0; if (channels != 3) return 0;

	//Extrair a cor verde
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;

			data[pos] = 0;
			data[pos + 2] = 0;
		}
	}
	return 1;
}

int vc_rgb_get_blue(IVC* srcdst)
{
	unsigned char* data = (unsigned char*)srcdst->data;
	int width = srcdst->width; int height = srcdst->height;
	int bytesperline = srcdst->width * srcdst->channels;
	int channels = srcdst->channels;
	int x, y;
	long int pos;

	// Verifica��o de erros
	if ((srcdst->width <= 0) || (srcdst->height <= 0) || (srcdst->data == NULL)) return 0; if (channels != 3) return 0;
	if (channels != 3) return 0;

	//Extrair a cor azul
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;

			data[pos] = 0;
			data[pos+1] = 0;
		}
	}
	return 1;
}


//Extrair RGB e mudar para cinza
int vc_rgb_get_red_gray(IVC* srcdst)
{
	unsigned char* data = (unsigned char*)srcdst->data;
	int width = srcdst->width; int height = srcdst->height;
	int bytesperline = srcdst->width * srcdst->channels;
	int channels = srcdst->channels;
	int x, y;
	long int pos;

	// Verifica��o de erros
	if ((srcdst->width <= 0) || (srcdst->height <= 0) || (srcdst->data == NULL)) return 0; if (channels != 3) return 0;
	if (channels != 3) return 0;

	//Extrair a cor vermelha
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;

			data[pos + 1] = data[pos];
			data[pos + 2] = data[pos];
		}
	}
	return 1;
}

int vc_rgb_get_green_gray(IVC* srcdst)
{
	unsigned char* data = (unsigned char*)srcdst->data;
	int width = srcdst->width; int height = srcdst->height;
	int bytesperline = srcdst->width * srcdst->channels;
	int channels = srcdst->channels;
	int x, y;
	long int pos;

	// Verifica��o de erros
	if ((srcdst->width <= 0) || (srcdst->height <= 0) || (srcdst->data == NULL)) return 0; if (channels != 3) return 0;
	if (channels != 3) return 0;

	//Extrair a cor vermelha
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;

			data[pos] = data[pos+1];
			data[pos + 2] = data[pos+1];
		}
	}
	return 1;
}

int vc_rgb_get_blue_gray(IVC* srcdst)
{
	unsigned char* data = (unsigned char*)srcdst->data;
	int width = srcdst->width; int height = srcdst->height;
	int bytesperline = srcdst->width * srcdst->channels;
	int channels = srcdst->channels;
	int x, y;
	long int pos;

	// Verifica��o de erros
	if ((srcdst->width <= 0) || (srcdst->height <= 0) || (srcdst->data == NULL)) return 0; if (channels != 3) return 0;
	if (channels != 3) return 0;

	//Extrair a cor vermelha
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;

			data[pos] = data[pos+2];
			data[pos + 1] = data[pos+2];
		}
	}
	return 1;
}

int vc_rgb_to_gray(IVC* src, IVC* dst)
{
	unsigned char *datasrc = (unsigned char *) src->data;
	int bytesperline_src = src->width * src->channels;
	int channels_src = src->channels;
	unsigned char *datadst = (unsigned char *) dst->data;
	int bytesperline_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;
	int width = src->width;
	int height = src->height;
	int x, y;
	long int pos_src, pos_dst;
	float rf, gf, bf;

	// Verifica��o de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL)) return 0;
	if ((src->width != dst->width) || (src->height != dst->height )) return 0;
	if((src->channels != 3) || (dst->channels != 1)) return 0;

		for (y = 0; y < height; y++)
		{
			for (x = 0; x < width; x++)
			{
				pos_src = y * bytesperline_src + x * channels_src;
				pos_dst = y * bytesperline_dst + x * channels_dst;

				rf = (float) datasrc[pos_src];
				gf = (float) datasrc[pos_src + 1];
				bf = (float) datasrc[pos_src + 2];

				datadst[pos_dst] = (unsigned char) ((rf * 0.299) + (gf * 0.587) + (bf * 0.114));
			}
		}
	return 1;
}

int vc_rgb_to_hsv(IVC* src, IVC* dst)
{
	unsigned char* data = (unsigned char*)src->data;
	int width = src->width;
	int height = src->height;
	int bytesperline = src->width * src->channels;
	int channels = src->channels;

	unsigned char* data_dst = (unsigned char*)dst->data;
	int width_dst = dst->width;
	int height_dst = dst->height;
	int bytesperline_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;

	int x, y, pos_src, pos_dst;
	float r, g, b, h, s, v, cmax, cmin, diff;

	// Cofere erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL))
		return 0;
	if ((src->width != dst->width) || (src->height != dst->height))
		return 0;
	if ((src->channels != 3) || (dst->channels != 3))
		return 0;

	// Transforma de RGB para HSV
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos_src = y * bytesperline + x * channels;
			pos_dst = y * bytesperline_dst + x * channels_dst;

			r = (float)data[pos_src];
			g = (float)data[pos_src + 1];
			b = (float)data[pos_src + 2];

			cmax = max(r, g, b);
			cmin = min(r, g, b);
			diff = cmax - cmin;

			// aplicar o de max a variavel V
			v = cmax;

			// confere os valores da s
			if (cmax == 0)
			{
				s = 0;
			}
			else
			{
				s = (float)((diff / cmax) * 255);
			}

			// confere os valores do h
			if (cmax == cmin)
			{
				h = 0;
			}
			else
			{
				if ((cmax == r) && (g >= b))
				{
					h = 43 * (g - b) / diff;
				}
				else if ((cmax == r) && (g < b))
				{
					h = 255 + (43 * (g - b) / diff);
				}
				else if (cmax == g)
				{
					h = 85 + (43 * (b - r) / diff);
				}
				else if (cmax == b)
				{
					h = 170 + (43 * (r - g) / diff);
				}
			}

			data_dst[pos_dst] = (unsigned char)(h);
			data_dst[pos_dst + 1] = (unsigned char)(s);
			data_dst[pos_dst + 2] = (unsigned char)(v);
		}
	}

	return 1;
}

//Calcula o maior de 3 valores
float max(float r, float g, float b)
{
	return ((r > g) ? (r > b ? r : b) : (g > b ? g : b));
}

// Calcula o menor de 3 valores
float min(float r, float g, float b)
{
	return ((r < g) ? (r < b ? r : b) : (g < b ? g : b));
}

int vc_hsv_segmentation(IVC* src, IVC* dst, int hmin, int hmax, int smin, int smax, int vmin, int vmax)
{
	unsigned char* data = (unsigned char*)src->data;
	int width = src->width;
	int height = src->height;
	int bytesperline = src->width * src->channels;
	int channels = src->channels;

	unsigned char* data_dst = (unsigned char*)dst->data;
	int width_dst = dst->width;
	int height_dst = dst->height;
	int bytesperline_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;

	int x, y, pos_src, pos_dst;
	float r, g, b, h, s, v, max, min, delta;

	// Cofere erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL))
		return 0;
	if ((src->width != dst->width) || (src->height != dst->height))
		return 0;
	if ((src->channels != 3) || (dst->channels != 1))
		return 0;

	// Faz a segmentação
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos_src = y * bytesperline + x * channels;
			pos_dst = y * bytesperline_dst + x * channels_dst;

			if ((data[pos_src] >= hmin && data[pos_src] <= hmax) && (data[pos_src + 1] >= smin && data[pos_src + 1] <= smax) && (data[pos_src + 2] >= vmin && data[pos_src + 2] <= vmax))
			{
				data_dst[pos_dst] = 255;
			}
			else
				data_dst[pos_dst] = 0;
		}
	}
	return 1;
}

// Transforma uma imagem em gray scale em rbg com tons cizentos
int vc_scale_gray_to_rgb(IVC *src, IVC *dst)
{
    unsigned char *data = (unsigned char *)src->data;
    int width = src->width;
    int height = src->height;
    int bytesperline = src->width * src->channels;
    int channels = src->channels;

    unsigned char *data_dst = (unsigned char *)dst->data;
    int width_dst = dst->width;
    int height_dst = dst->height;
    int bytesperline_dst = dst->width * dst->channels;
    int channels_dst = dst->channels;

    int x, y, pos_src, pos_dst;
    float r, g, b;

    // Cofere erros
    if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL))
        return 0;
    if ((src->width != dst->width) || (src->height != dst->height))
        return 0;
    if ((src->channels != 1) || (dst->channels != 3))
        return 0;

    // Faz a segmentaÃ§Ã£o
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            pos_src = y * bytesperline + x * channels;
            pos_dst = y * bytesperline_dst + x * channels_dst;

            if (data[pos_src] >= 0 && data[pos_src] <= 64)
            {
                data_dst[pos_dst] = 0;
                data_dst[pos_dst + 1] = data[pos_src] * 4;
                data_dst[pos_dst + 2] = 255;
            }
            else if (data[pos_src] > 64 && data[pos_src] <= 128)
            {
                data_dst[pos_dst] = 0;
                data_dst[pos_dst + 1] = 255;
                data_dst[pos_dst + 2] = data[pos_src] / 4;
            }
        }
    }
    return 1;
}

int vc_count_pixels(IVC *src)
{
    unsigned char *data = (unsigned char *)src->data;
    int width = src->width;
    int height = src->height;
    int bytesperline = src->width * src->channels;
    int channels = src->channels;

    int x, y, pos, count;

    // Cofere erros
    if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL))
        return 0;
    if (src->channels != 1)
        return 0;

    // Faz algo
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            pos = y * bytesperline + x * channels;

            if (data[pos] != 0)
            {
                count++;
            }
        }
    }
    return count;
}

/*
// , float *n[], float *a[]  possÃ­veis argumentos
int vc_brain_black_area(IVC *src, IVC *dst)
{
    unsigned char *data = (unsigned char *)src->data;
    int width = src->width;
    int height = src->height;
    int bytesperline = src->width * src->channels;
    int channels = src->channels;

    unsigned char *data_dst = (unsigned char *)dst->data;
    int width_dst = dst->width;
    int height_dst = dst->height;
    int bytesperline_dst = dst->width * dst->channels;
    int channels_dst = dst->channels;

    int pos_src, pos_dst;

    int x, y, z, pos, inicio, fim;
    bool pode_comecar, pode_terminar;
    float r, g, b, yellow;
    int rm = 0, gm = 0, bluem = 0, ym = 0, bm = 0;

    // Cofere erros
    if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL))
        return 0;
    if (src->channels != 3)
        return 0;

    // Faz algo
    for (y = 0; y < height; y++)
    {
        pode_comecar = true;
        pode_terminar = false;
        for (x = 0; x < (width / 2); x++)
        {
            pos = y * bytesperline + x * channels;

            if (data[pos] != 0 && data[pos + 1] != 0 && data[pos + 2] != 0)
            {
                inicio = pos;
                break;
            }
        }
        for (x = width - 21; x > (width / 2); x--)
        {
            pos = y * bytesperline + x * channels;

            if (data[pos] != 0 && data[pos + 1] != 0 && data[pos + 2] != 0)
            {
                fim = pos;
                break;
            }
        }
        for (x = 0; x < width; x++)
        {
            pos_src = y * bytesperline + x * channels;
            pos_dst = y * bytesperline_dst + x * channels_dst;

            if (pos_dst >= inicio / 3 && pos_dst / 3 <= fim)
            {
                data_dst[pos_dst] = 255;
            }
        }
    }
    return 1;
}
 */

int vc_gray_to_binary(IVC *src, IVC *dst, int threshold)
{
    unsigned char *data = (unsigned char *)src->data;
    int width = src->width;
    int height = src->height;
    int bytesperline = src->width * src->channels;
    int channels = src->channels;

    unsigned char *data_dst = (unsigned char *)dst->data;
    int width_dst = dst->width;
    int height_dst = dst->height;
    int bytesperline_dst = dst->width * dst->channels;
    int channels_dst = dst->channels;

    int pos_src, pos_dst, x, y;

    // Cofere erros
    if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL))
        return 0;
    if ((dst->width <= 0) || (dst->height <= 0))
        return 0;
    if ((src->channels != 1) || (dst->channels != 1))
        return 0;

    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            pos_src = y * bytesperline + x * channels;
            pos_dst = y * bytesperline_dst + x * channels_dst;

            if (data[pos_src] > threshold)
            {
                data_dst[pos_dst] = 255;
            }
            else if (data[pos_src] <= threshold)
            {
                data_dst[pos_dst] = 0;
            }
        }
    }
    return 1;
}

int vc_gray_to_binary_global_mean(IVC *src, IVC *dst)
{
    unsigned char *data = (unsigned char *)src->data;
    int width = src->width;
    int height = src->height;
    int bytesperline = src->width * src->channels;
    int channels = src->channels;

    unsigned char *data_dst = (unsigned char *)dst->data;
    int width_dst = dst->width;
    int height_dst = dst->height;
    int bytesperline_dst = dst->width * dst->channels;
    int channels_dst = dst->channels;

    int pos_src, pos_dst, x, y, z;
    float media;
    int soma;

    // Cofere erros
    if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL))
        return 0;
    if ((dst->width <= 0) || (dst->height <= 0))
        return 0;
    if ((src->channels != 1) || (dst->channels != 1))
        return 0;

    for (z = 0; z < 2; z++)
    {
        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                pos_src = y * bytesperline + x * channels;
                pos_dst = y * bytesperline_dst + x * channels_dst;

                if (z == 0)
                {
                    soma += data[pos_src];
                }
                else if (z == 1)
                {
                    if (data[pos_src] > (int)media)
                    {
                        data_dst[pos_dst] = 255;
                    }
                    else if (data[pos_src] <= (int)media)
                    {
                        data_dst[pos_dst] = 0;
                    }
                }
            }
        }
        media = soma / (width * height);
        printf("media %f, soma %d\n", media, soma);
    }
    return 1;
}

int vc_gray_to_binary_kernel_8(IVC *src, IVC *dst)
{
    unsigned char *data = (unsigned char *)src->data;
    int width = src->width;
    int height = src->height;
    int bytesperline = src->width * src->channels;
    int channels = src->channels;

    unsigned char *data_dst = (unsigned char *)dst->data;
    int width_dst = dst->width;
    int height_dst = dst->height;
    int bytesperline_dst = dst->width * dst->channels;
    int channels_dst = dst->channels;

    int pos_src, pos_dst, x, y, z;
    int soma = 0, media;

    // Cofere erros
    if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL))
        return 0;
    if ((dst->width <= 0) || (dst->height <= 0))
        return 0;
    if ((src->channels != 1) || (dst->channels != 1))
        return 0;

    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            pos_src = y * bytesperline + x * channels;
            pos_dst = y * bytesperline_dst + x * channels_dst;
            soma = 0;

            // {x}{ }{ }
            // { }{ }{ }
            // { }{ }{ }
            soma += (((pos_src - (width * y)) == 0) || (y == 0)) ? 0 : data[pos_src - 1 - width];
            // { }{x}{ }
            // { }{ }{ }
            // { }{ }{ }
            soma += y == 0 ? 0 : data[pos_src - width];
            // { }{ }{x}
            // { }{ }{ }
            // { }{ }{ }
            soma += ((width * (y + 1)) % ((pos_src + 1)) == 0 || (y == 0)) ? 0 : data[pos_src + 1 - width];

            // { }{ }{ }
            // {x}{ }{ }
            // { }{ }{ }
            soma += ((pos_src - (width * y)) == 0) ? 0 : data[pos_src - 1];
            // { }{ }{ }
            // { }{x}{ }
            // { }{ }{ }
            soma += data[pos_src];
            // { }{ }{ }
            // { }{ }{x}
            // { }{ }{ }
            soma += (width * (y + 1)) % ((pos_src + 1)) == 0 ? 0 : data[pos_src + 1];

            // { }{ }{ }
            // { }{ }{ }
            // {x}{ }{ }
            soma += ((pos_src - (width * y)) == 0) || (y == (width - 1)) ? 0 : data[pos_src - 1 + width];
            // { }{ }{ }
            // { }{ }{ }
            // { }{x}{ }
            soma += (y == (width - 1)) ? 0 : data[pos_src];
            // { }{ }{ }
            // { }{ }{ }
            // { }{ }{x}
            soma += ((width * (y + 1)) % ((pos_src + 1)) == 0 || (y == (width - 1))) ? 0 : data[pos_src + 1 + width];

            media = soma / 9;
            // printf("Media: %d\n%d %d %d\n%d %d %d\n%d %d %d\n%d\n", media, data[pos_src - 1 - width], data[pos_src - width], data[pos_src + 1 - width], data[pos_src - 1], data[pos_src], data[pos_src + 1], data[pos_src - 1 + width], data[pos_src + width], data[pos_src + 1 + width], soma);

            if (data[pos_src] > media)
            {
                data_dst[pos_dst] = 255;
            }
            else if (data[pos_src] <= media)
            {
                data_dst[pos_dst] = 0;
            }
        }
    }
    return 1;
}

int vc_gray_to_binary_ad(IVC *src, IVC *dst, int t)
{
    unsigned char *data = (unsigned char *)src->data;
    int width = src->width;
    int height = src->height;
    int bytesperline = src->width * src->channels;
    int channels = src->channels;

    unsigned char *data_dst = (unsigned char *)dst->data;
    int width_dst = dst->width;
    int height_dst = dst->height;
    int bytesperline_dst = dst->width * dst->channels;
    int channels_dst = dst->channels;

    int pos_src, pos_dst, pos_x, pos_k, x, y, z, xx, yy;
    int soma = 0, media;

    if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL))
        return 0;
    if ((dst->width <= 0) || (dst->height <= 0))
        return 0;
    if ((src->channels != 1) || (dst->channels != 1))
        return 0;

    for (y = 0; y <= height; y++)
    {
        for (x = 0; x <= width; x++)
        {
            pos_src = y * bytesperline + x * channels;
            pos_dst = y * bytesperline_dst + x * channels_dst;

            for (yy = -(t - 1) / 2; yy <= (t - 1) / 2; yy++)
            {
                for (xx = -(t - 1) / 2; xx <= (t - 1) / 2; xx++)
                {
                    pos_k = ((yy + y) * bytesperline) + ((xx + x) * channels);

                    // if ((pos_src - (y * bytesperline) + xx) < 0 || (pos_src - (y * bytesperline) + xx) >= width || (pos_src + bytesperline * yy))
                }
            }
        }
    }
    return 1;
}

int vc_gray_histogram_show(IVC *src, IVC *dst)
{
    unsigned char* data = (unsigned char*)src->data;
    int width = src->width;
    int height = src->height;
    int bytesperline = src->width * src->channels;
    int channels = src->channels;

    unsigned char* data_dst = (unsigned char*)dst->data;
    int width_dst = dst->width;
    int height_dst = dst->height;
    int bytesperline_dst = dst->width * dst->channels;
    int channels_dst = dst->channels;

    int x, y, pos_src, pos_dst;


}

/*
 * // Gera o gr�fico com o histograma
for (i = 0, x = (width - 256) / 2; i<256; i++, x++)
{
	for (y = height - 1; y>height - 1 - normhist[i] * height; y--)
	{
		datadst[y * bytesperline + x * channels] = 255;
	}
}

// Desenha linhas de in�cio (itensidade = 0) e fim (intensidade = 255)
for (y = 0; y<height - 1; y++)
{
	datadst[y * bytesperline + ((width - 256) / 2 - 1) * channels] = 127;
	datadst[y * bytesperline + ((width + 256) / 2 + 1) * channels] = 127;
}
*/
 