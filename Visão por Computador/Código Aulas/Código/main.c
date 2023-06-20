#include <stdio.h>
#include "vc.h"

// Abrir imagem, alterar e gravar em novo ficheiro
int main(void)
{
    IVC *image_src;
    IVC *image_dst;
    IVC* image_dst2;

    image_src = vc_read_image("C:/Users/morei/Desktop/VC/Images/PET-Normal.ppm");
    if (image_src == NULL) {
        printf("ERROR -> vc_read_image():\n\tFile not found! \n");
        getchar();
        return 0;
    }

    image_dst = vc_image_new(image_src->width, image_src->height,3 , image_src->levels);
    image_dst2 = vc_image_new(image_src->width, image_src->height, 1, image_src->levels);



    vc_write_image("C:/Users/morei/desktop/VC/Aulas/OutputHSV.ppm", image_dst);
    vc_write_image("C:/Users/morei/desktop/VC/Aulas/OutputSegmentado.pgm", image_dst2);


    /*
    system("cmd /c start C:/Users/morei/Desktop/VC/Aulas/FilterGear.exe C:/Users/morei/Desktop/VC/Images/PET-NORMAL.ppm"); // Input
    system("C:/Users/morei/Desktop/VC/Aulas/FilterGear.exe C:/Users/morei/Desktop/VC/Aulas/OutputSegmentado.pgm"); // Output
    */

    vc_image_free(image_src);
    vc_image_free(image_dst);
    vc_image_free(image_dst2);

    //printf("Press any key to exit... In");
    //getchar();

    return 0;
}

/* SEGMENTAÇÃO
vc_hsv_segmentation(image_dst, image_dst2, 21, 63, 0, 255, 0, 255); //30-70; 50-100; 60-100
*/

