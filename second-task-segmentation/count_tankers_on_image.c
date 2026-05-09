#include <stdio.h>
#include <stdlib.h> 
#include <strings.h>
#include <math.h> 
#include "lodepng.h" 

typedef struct pixel {
    int x;
    int y;

} pixel;

typedef struct queue_element {
    
    pixel *pixel;

    struct queue_element *next;

} queue_element;

typedef struct queue {

    queue_element *head;
    queue_element *tail;
    int size;

} queue;

void push(queue *bfs_queue, int x, int y) {
    queue_element *new_element;
    pixel *pixel_to_push;

    pixel_to_push = (pixel*) malloc(sizeof(pixel));
    pixel_to_push->x = x;
    pixel_to_push->y = y;
    
    new_element = (queue_element*) malloc(sizeof(queue_element));

    new_element->next = NULL;
    new_element->pixel = pixel_to_push;

    if (bfs_queue->size > 0) {
        bfs_queue->tail->next = new_element;
        bfs_queue->tail = new_element;
    } else {
        bfs_queue->head = bfs_queue->tail = new_element;
    }

    bfs_queue->size += 1;
}

pixel* pop(queue *bfs_queue) {
    pixel *result = NULL;
    queue_element *head;

    if (bfs_queue->size > 0) {
        head = bfs_queue->head;

        result = head->pixel;
        if (bfs_queue->size == 1) bfs_queue->tail = NULL;
        bfs_queue->head = head->next;
        bfs_queue->size -= 1;
        
        free(head);

    }

    return result;
}

int is_not_empty(queue *bfs_queue) {
    return (bfs_queue->size > 0);
}

unsigned char* load_png(const char* filename, unsigned int* width, unsigned int* height)  {
  unsigned char* image = NULL; 
  int error = lodepng_decode32_file(&image, width, height, filename);
  if(error != 0) {
    printf("error %u: %s\n", error, lodepng_error_text(error)); 
  }
  return (image);
}

void write_png(const char* filename, const unsigned char* image, unsigned width, unsigned height) {
  unsigned char* png;
  long unsigned int pngsize;

  int error = lodepng_encode32(&png, &pngsize, image, width, height);

  if(error == 0) {
      lodepng_save_file(png, pngsize, filename);
  } else { 
    printf("error %u: %s\n", error, lodepng_error_text(error));
  }
  free(png);
}

void write_wb_png(const char* filename, const unsigned char* image, unsigned width, unsigned height) {
    
    int error = lodepng_encode_file(filename, image, width, height, LCT_GREY, 8);
    
    if (error) {
        printf("error %u: %s\n", error, lodepng_error_text(error));
    }
}

void cast_to_black_and_white(unsigned char *input_image, unsigned char *output_image, unsigned width, unsigned height) {
    unsigned index_in_rgba_representation;
    unsigned i, j;

    unsigned char red, blue, green, gray;

    for (i=0; i<height; i++) {
        for (j=0; j<width; j++) {
            index_in_rgba_representation = (i*width + j)*4;

            red = input_image[index_in_rgba_representation];
            green = input_image[index_in_rgba_representation+1];
            blue = input_image[index_in_rgba_representation+2];

            gray = ((red+green+blue) / 3);

            output_image[i*width + j] = gray;
        }
    }


}

void contrast(unsigned char *image, int bw_size) { 
    int i; 

    for(i=0; i < bw_size; i++) {

        if(image[i] <= 70) image[i] = 0; 

        if(image[i] > 70) image[i] = 255;
    } 
} 

void gauss_blur(unsigned char *input_image, unsigned char *output_image, int width, int height) { 
    unsigned index_in_rgba_representation;
    unsigned i, j;

    unsigned char red, blue, green, gray;

    for (i=4; i<height-4; i++) {
        for (j=4; j<width-4; j++) {

            output_image[width*i+j] = 0.084*input_image[(width*i+j)*4] + 0.084*input_image[(width*(i+1)+j)*4] + 0.084*input_image[(width*(i-1)+j)*4]; 
            output_image[width*i+j] = output_image[width*i+j] + 0.084*input_image[(width*i+(j+1))*4] + 0.084*input_image[(width*i+(j-1))*4]; 
            output_image[width*i+j] = output_image[width*i+j] + 0.063*input_image[(width*(i+1)+(j+1))*4] + 0.063*input_image[(width*(i+1)+(j-1))*4]; 
            output_image[width*i+j] = output_image[width*i+j] + 0.063*input_image[(width*(i-1)+(j+1))*4] + 0.063*input_image[(width*(i-1)+(j-1))*4]; 
        }
    }

}

void copy_image(unsigned char *from, unsigned char *to, int size) {
    int i;

    for (i=0; i<size; i++) to[i] = from[i];

}

void mark_tanker_on_image(unsigned char *image, pixel *left_bottom_pixel, pixel *right_top_pixel, int width) {

    unsigned index_in_rgba_representation;
    unsigned i;

    for (i=left_bottom_pixel->x-2; i<right_top_pixel->x+2; i++) {
        index_in_rgba_representation = (i*width + left_bottom_pixel->y)*4;

        image[index_in_rgba_representation] = 254;
        image[index_in_rgba_representation+1] = 0;
        image[index_in_rgba_representation+2] = 0;

    }

    for (i=left_bottom_pixel->x-2; i<right_top_pixel->x+2; i++) {
        index_in_rgba_representation = (i*width + right_top_pixel->y)*4;

        image[index_in_rgba_representation] = 254;
        image[index_in_rgba_representation+1] = 0;
        image[index_in_rgba_representation+2] = 0;

    }

    for (i=right_top_pixel->y-2; i<left_bottom_pixel->y+2; i++) {
        index_in_rgba_representation = ((left_bottom_pixel->x)*width + i)*4;

        image[index_in_rgba_representation] = 254;
        image[index_in_rgba_representation+1] = 0;
        image[index_in_rgba_representation+2] = 0;

    }

    for (i=right_top_pixel->y-2; i<left_bottom_pixel->y+2; i++) {
        index_in_rgba_representation = ((right_top_pixel->x)*width + i)*4;

        image[index_in_rgba_representation] = 254;
        image[index_in_rgba_representation+1] = 0;
        image[index_in_rgba_representation+2] = 0;

    }

}

int breadth_first_search(unsigned char *image_to_mark_tankers, unsigned char *processed_image, int *is_pixel_lit, int x, int y, int width, int height) {

    queue *bfs_queue;

    float edges_ratio;

    pixel *left_bottom_pixel=NULL, *right_top_pixel=NULL, *tmp;
    pixel *current_pixel;

    int square_of_object = 0;
    int square_of_object_bounding_box=0;
    int shift_on_x, shift_on_y;
    int shifted_x, shifted_y;

    if (x < 272 && y > 700) return 0;
    if (x > 153 && y < 373) return 0;
    if (x < 70 && y < 363) return 0;

    bfs_queue = (queue*) malloc(sizeof(queue));
    bfs_queue->size = 0;
    bfs_queue->head = NULL;
    bfs_queue->tail = NULL;

    push(bfs_queue, x, y);

    while (is_not_empty(bfs_queue)) {
        
        current_pixel = pop(bfs_queue);
        square_of_object++;

        for (shift_on_x=-1; shift_on_x<=1; shift_on_x++) {
            for (shift_on_y=-1; shift_on_y<=1; shift_on_y++) {
                if (!((shift_on_x == 0) && (shift_on_y == 0)) || (shift_on_x*shift_on_y != 0)) {
                    shifted_x = current_pixel->x + shift_on_x;
                    shifted_y = current_pixel->y + shift_on_y;

                    if ((0<=shifted_x) && (shifted_x<=height) && (0<=shifted_y) && (shifted_y<=width) && (processed_image[shifted_x*width+shifted_y]!=0) && (!is_pixel_lit[shifted_x*width+shifted_y])) {
                        is_pixel_lit[shifted_x*width+shifted_y] = 1;
                        push(bfs_queue, shifted_x, shifted_y);
                    }
                }
            }
        }

        if ((left_bottom_pixel == NULL) || (left_bottom_pixel->y < current_pixel->y) || (left_bottom_pixel->y == current_pixel->y && left_bottom_pixel->x>current_pixel->x)) {
                
            tmp = left_bottom_pixel;
            left_bottom_pixel = current_pixel;
            if (tmp != NULL) free(tmp);

        } else if ((right_top_pixel == NULL) || (right_top_pixel->y > current_pixel->y) || (right_top_pixel->y == current_pixel->y && right_top_pixel->x<current_pixel->x)) {
            
            tmp = right_top_pixel;
            right_top_pixel = current_pixel;
            if (tmp != NULL) free(tmp);

        } else {
            free(current_pixel);
        }

    }

    free(bfs_queue);
    
    if (left_bottom_pixel != NULL && right_top_pixel != NULL && square_of_object) {
        square_of_object_bounding_box = (right_top_pixel->x - left_bottom_pixel->x)*(left_bottom_pixel->y - right_top_pixel->y);
        
        if (square_of_object<50 && square_of_object_bounding_box<50) {
            mark_tanker_on_image(image_to_mark_tankers, left_bottom_pixel, right_top_pixel, width);
            return 1;
        }
    }

    return 0;

}

int get_tankers(unsigned char *image_to_mark_tankers, unsigned char *bw_contrast_image, int *is_pixel_lit, int width, int height) {
    int result_count_of_tankers = 0;
    int i, j;

    for (i=0; i<height; i++) {
        for (j=0; j<width; j++) {
            if (bw_contrast_image[i*width+j] != 0 && !is_pixel_lit[i*width+j]) {
                result_count_of_tankers += breadth_first_search(image_to_mark_tankers, bw_contrast_image, is_pixel_lit, i, j, width, height);
            }
        }
    }

    return result_count_of_tankers;
}
  
int main() { 

    unsigned char *bw_image, *blr_pic, *finish;

    unsigned int width, height;

    int *is_pixel_lit;
    int size, bw_size;
    
    unsigned char* picture = load_png("t.png", &width, &height); 
    if (picture == NULL) { 
        printf("Problem reading picture from the file %s. Error.\n", "t.png"); 
        return -1; 
    } 

    size = width * height * 4;
    bw_size = width * height;
    
    
    bw_image = (unsigned char*)malloc(bw_size*sizeof(unsigned char)); 
    blr_pic = (unsigned char*)malloc(bw_size*sizeof(unsigned char)); 
    finish = (unsigned char*)malloc(size*sizeof(unsigned char)); 

    is_pixel_lit = (int*) calloc(bw_size, sizeof(int));

    copy_image(picture, finish, size);

    cast_to_black_and_white(picture, bw_image, width, height);
    write_wb_png("picture_after_casting_to_white_and_black.png", bw_image, width, height);

    contrast(bw_image, bw_size);
    write_wb_png("picture_after_applying_contrast.png", bw_image, width, height);

    printf("Total count of found tankers: %d", get_tankers(finish, bw_image, is_pixel_lit, width, height));
    write_png("input_picture_after_marking_tankers.png", finish, width, height);
    
    free(bw_image); 
    free(blr_pic); 
    free(finish); 
    free(picture); 
    free(is_pixel_lit);
    
    return 0; 
}
