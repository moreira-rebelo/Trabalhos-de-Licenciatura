/*
--------------------------------------------------------------
|           INSTITUTO POLITECNICO DO CAVADO E DO AVE         |
|                         2022/2023							 |
|															 |
|          ENGENHARIA DE SISTEMAS INFORMATICOS - PL			 |
|                   VISAO POR COMPUTADOR					 |
|															 |
|				     TRABALHO PRATICO						 |
|															 |
|				 DIOGO PARENTE - a23525                      |
|			   FRANCISCO REBELO - a16443                     |
|			   GABRIEL JABLONSKI - a21958					 |
|															 |
|				BARCELOS, MAIO DE 2023						 |
--------------------------------------------------------------
*/

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <chrono>
#include <opencv2\opencv.hpp>
#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\videoio.hpp>
#include <stdio.h>
#include <opencv2/imgproc.hpp>


extern "C" {
#include "vc.h"
}


void vc_timer(void) {
	static bool running = false;
	static std::chrono::steady_clock::time_point previousTime = std::chrono::steady_clock::now();

	if (!running) {
		running = true;
	}
	else {
		std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
		std::chrono::steady_clock::duration elapsedTime = currentTime - previousTime;

		// Tempo em segundos.
		std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(elapsedTime);
		double nseconds = time_span.count();

		std::cout << "Tempo decorrido: " << nseconds << "segundos" << std::endl;
		std::cout << "Pressione qualquer tecla para continuar...\n";
		std::cin.get();
	}
}


int main(void) {

#pragma region Declara��o de variaveis e arrays

	// V�deo
	char videofile[20] = "video.avi";
	cv::VideoCapture capture;
	struct
	{
		int width, height;
		int ntotalframes;
		int fps;
		int nframe;
	} video;

	// Outros
	std::string str;
	int key = 0;

	SINAL sinal;
	int areaAzul;
	int areaVermelha;
	int textoR, textoG, textoB;
	int numBlobs = 0;
	OVC* blobs = (OVC*)malloc(sizeof(OVC));
	int altura, largura;

#pragma endregion



	//------------------------------ Leitura de v�deo de um ficheiro ou c�mera ------------------------------
	//Comentar a linha 75 e descomentar a 77 para utilizar a c�mera, fazer o inverso para utilizar um v�deo

	capture.open(videofile); //O ficheiro video.avi dever� estar localizado no mesmo direct�rio que o ficheiro de c�digo fonte.
	
	//capture.open(0, cv::CAP_DSHOW); //C�mera

#pragma region OpenCV (Obtencao de propriedades do v�deo e iniciacao da exibicao da imagem)

	/* Verifica se foi poss�vel abrir o ficheiro de v�deo */
	if (!capture.isOpened())
	{
		std::cerr << "Erro ao abrir o ficheiro de v�deo!\n";
		return 1;
	}

	/* N�mero total de frames no v�deo */
	video.ntotalframes = (int)capture.get(cv::CAP_PROP_FRAME_COUNT);
	/* Frame rate do v�deo */
	video.fps = (int)capture.get(cv::CAP_PROP_FPS);
	/* Resolu��o do v�deo */
	video.width = (int)capture.get(cv::CAP_PROP_FRAME_WIDTH);
	video.height = (int)capture.get(cv::CAP_PROP_FRAME_HEIGHT);

	/* Cria uma janela para exibir o v�deo */
	cv::namedWindow("VC - VIDEO", cv::WINDOW_AUTOSIZE);

	/* Inicia o timer */
	vc_timer();

	cv::Mat frame;
	while (key != 'q') {
		/* Leitura de uma frame do v�deo */
		capture.read(frame);

		/* Verifica se conseguiu ler a frame */
		if (frame.empty()) break;

		/* N�mero da frame a processar */
		video.nframe = (int)capture.get(cv::CAP_PROP_POS_FRAMES);

#pragma endregion

		// Cria uma nova imagem IVC
		IVC *image = vc_image_new(video.width, video.height, 3, 255);
		IVC* imageOriginal = vc_image_new(video.width, video.height, 3, 255);

		// Copia dados de imagem da estrutura cv::Mat para uma estrutura IVC
		memcpy(imageOriginal->data, frame.data, video.width * video.height * 3); //Backup da imagem original
		memcpy(image->data, frame.data, video.width* video.height * 3); //Imagem a ser trabalhada

		//Aloca mem�ria para guardar as imagens a manipular
		IVC* imageHsv = vc_image_new(image->width, image->height, 3, image->levels);
		IVC* imageSegVerm = vc_image_new(image->width, image->height, 3, image->levels);
		IVC* imageSegAzu = vc_image_new(image->width, image->height, 3, image->levels);
		IVC* imageGray = vc_image_new(image->width, image->height, 1, image->levels);
		IVC* imageBin1 = vc_image_new(image->width, image->height, 1, 1);
		IVC* imageBin2 = vc_image_new(image->width, image->height, 1, 1);
		IVC* imageBlob = vc_image_new(image->width, image->height, 1, 255);
		IVC* imageRgb = vc_image_new(image->width, image->height, 3, image->levels);

		//-------------------- Algoritmo e fun��es para identifica��o dos sinais --------------------

		//Calcula a metade da resolu��o da c�mara ou do v�deo
			altura = video.height / 2;
			largura = video.width / 2;
		

		vc_rgb_to_hsv(image, imageHsv); //Convers�o da imagem RGB para HSV
		vc_hsv_segmentation(imageHsv, imageSegAzu, imageSegVerm, 0, 40, 50, 100, 0, 100, 216, 262, 62, 100, 0, 100); //Segmenta a imagem de entrada em 2 imagens com par�metros de filtragem de cor diferentes

		areaAzul = vc_count_pixels(imageSegAzu); //Calcula a quantidade de pixeis a vermelho
		areaVermelha = vc_count_pixels(imageSegVerm); //Calcula a quantidade de pixeis a azul

		if ((areaAzul > 100) && (areaVermelha < 100)) { //          ---------------------------- AZUIS ----------------------------
			strcpy(sinal.nomeCor, "Azul"); //Muda a cor do texto e o texto em si
			textoR = 0;
			textoG = 0;
			textoB = 255;
			
			vc_hsv_to_grayscale(imageSegAzu, imageGray); //Converte de HSV para escala de cinza

			vc_gray_to_binary_global_mean(imageGray, imageBin1); //Converte para imagem bin�ria
			
			vc_binary_erode(imageBin1, imageBin2, 15); //Eros�o para remover contornos ou lixo
			
			vc_binary_blob_labelling(imageBin2, imageBlob, &numBlobs); //Labelling de blobs

			vc_get_blob_properties(imageBlob, blobs, &numBlobs); //Obt�m propriedades dos blobs

			vc_draw_boundingbox(image, blobs); //Desenha a bounding box

			vc_draw_centerofgravity(image, blobs); //Desenha o centro de gravidade

			if ((numBlobs == 1) && (blobs[0].yc < altura) && (blobs[0].xc < largura)) { //Se o sinal tiver 1 blob e o centro de gravidade estiver compreendindo entre os valores definidos, ent�o:
				strcpy(sinal.nomeSinal, "H24 - Auto-estrada");
			}
			else if ((numBlobs == 1) && (blobs[0].xc < largura)){ //Se o sinal tiver 1 blob e o centro de gravidade estiver compreendindo entre os valores definidos, ent�o:
				strcpy(sinal.nomeSinal, "D1a - Sentido obrigatorio (Direita)");
			}
			else if ((numBlobs == 1) && (blobs[0].xc > largura)) { //Se o sinal tiver 1 blob e o centro de gravidade estiver compreendindo entre os valores definidos, ent�o:
				strcpy(sinal.nomeSinal, "D1b - Sentido obrigatorio (Esquerda)");
			}
			else if (numBlobs == 4) { //Se o sinal tiver 4 blobs, ent�o:
				strcpy(sinal.nomeSinal, "H25 - Via reservada a automoveis e motociclos");
			}

			//vc_binary_to_gray(imageBlob, imageGray); //Usar para testes
			//vc_scale_gray_to_red(imageGray, imageRgb); //Usar para testes
		}

		else if ((areaVermelha > 100) && (areaAzul < 100)) { //        ---------------------------- VERMELHOS ----------------------------
			strcpy(sinal.nomeCor, "Vermelho"); //Muda a cor do texto e o texto em si
			textoR = 255;
			textoG = 0;
			textoB = 0;
			
			vc_hsv_to_grayscale(imageSegVerm, imageGray);//Converte de HSV para escala de cinza

			vc_gray_to_binary_global_mean(imageGray, imageBin1); //Converte para imagem bin�ria
			
			vc_binary_erode(imageBin1, imageBin2, 15); //Eros�o para remover contornos ou lixo
			
			vc_binary_blob_labelling(imageBin2, imageBlob, &numBlobs); //Labelling de blobs

			vc_get_blob_properties(imageBlob, blobs, &numBlobs); //Obt�m propriedades dos blobs

			vc_draw_boundingbox(image, blobs); //Desenha a bounding box

			vc_draw_centerofgravity(image, blobs);//Desenha o centro de gravidade

			if (numBlobs == 1) {  //Se o sinal tiver 1 blob, ent�o:
				strcpy(sinal.nomeSinal, "C1 - Sentido Proibido");
			}
			else if (numBlobs == 3) { //Se o sinal tiver 3 blob, ent�o:
				strcpy(sinal.nomeSinal, "B2 - STOP");
			}
			
			//vc_binary_to_gray(imageBlob, imageGray); //Usar para testes
			//vc_scale_gray_to_red(imageGray, imageRgb); //Usar para testes
		}

		else { //                                                        ---------------------------- OUTROS ----------------------------
			//Caso o sinal n�o seja nem vermelho nem azul:
			strcpy(sinal.nomeCor, "??");
			strcpy(sinal.nomeSinal, "Desconhecido");
			textoR = 255;
			textoG = 255;
			textoB = 255;
		}

		//printf("Blobs: %d\n", numBlobs); //Usar para testes
		
		// Copia dados de imagem da estrutura IVC para uma estrutura cv::Mat
		memcpy(frame.data, image->data, video.width * video.height * 3);

		// Liberta a mem�ria da imagem IVC que havia sido criada
		vc_image_free(image);
		vc_image_free(imageHsv);
		vc_image_free(imageSegAzu);
		vc_image_free(imageSegVerm);
		vc_image_free(imageOriginal);
		vc_image_free(imageGray);
		vc_image_free(imageBin1);
		vc_image_free(imageBin2);
		vc_image_free(imageBlob);

#pragma region OpenVC (Inser��o de texto na imagem, exibicao da imagem e encerramento do programa)

		/* Inser��o de texto na frame */
		str = std::string("RESOLUCAO: ").append(std::to_string(video.width)).append("x").append(std::to_string(video.height));
		cv::putText(frame, str, cv::Point(20, 25), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);
		cv::putText(frame, str, cv::Point(20, 25), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 1);
		str = std::string("TOTAL DE FRAMES: ").append(std::to_string(video.ntotalframes));
		cv::putText(frame, str, cv::Point(20, 50), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);
		cv::putText(frame, str, cv::Point(20, 50), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 1);
		str = std::string("FRAME RATE: ").append(std::to_string(video.fps));
		cv::putText(frame, str, cv::Point(20, 75), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);
		cv::putText(frame, str, cv::Point(20, 75), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 1);
		str = std::string("N. DA FRAME: ").append(std::to_string(video.nframe));
		cv::putText(frame, str, cv::Point(20, 100), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);
		cv::putText(frame, str, cv::Point(20, 100), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 1);
		str = std::string("COR DO SINAL: ").append(sinal.nomeCor);
		cv::putText(frame, str, cv::Point(20, 125), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2); 
		cv::putText(frame, str, cv::Point(20, 125), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(textoB, textoG, textoR), 1);
		str = std::string("NOME DO SINAL: ").append(sinal.nomeSinal);
		cv::putText(frame, str, cv::Point(20, 150), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);
		cv::putText(frame, str, cv::Point(20, 150), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 1);


		/* Exibe a frame */
		cv::imshow("VC - VIDEO", frame);

		/* Sai da aplica��o, se o utilizador premir a tecla 'q' */
		key = cv::waitKey(1);
	}

	/* Para o timer e exibe o tempo decorrido */
	vc_timer();

	/* Fecha a janela */
	cv::destroyWindow("VC - VIDEO");

	/* Fecha o ficheiro de v�deo */
	capture.release();

#pragma endregion

	return 0;
}