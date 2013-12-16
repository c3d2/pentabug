#include <stdlib.h>

#include <pentabug/music.h>
#include <pentabug/app.h>
#include <pentabug/hal.h>
#include <pentabug/matrix.h>

static uint8_t randm = 0;

static void init(void) {
    matrix_start();
}
static void wuerfel(void) {

    void w_eins(void){
        matrix_set(1,1,1);
    }
    void w_zwei(void){
        matrix_set(0,0,1);
        matrix_set(2,2,2);
    }
    void w_drei(void){
        matrix_set(0,0,1);
        matrix_set(2,2,1);
        matrix_set(1,1,1);
    }
    void w_vier(void){
        matrix_set(0,0,1);
        matrix_set(0,2,1);
        matrix_set(2,2,1);
        matrix_set(2,0,1);
    }
    void w_funf(void){
        matrix_set(1,1,1);
        matrix_set(0,0,1);
        matrix_set(0,2,1);
        matrix_set(2,2,1);
        matrix_set(2,0,1);
    }
    void w_sechs(void){
        matrix_set(1,0,1);
        matrix_set(0,0,1);
        matrix_set(0,2,1);
        matrix_set(2,2,1);
        matrix_set(2,0,1);
        matrix_set(1,2,1);
    }
    void clear(void){
        for(uint8_t x = 0; x < 3; ++x){
            for(uint8_t y = 0; y < 3; ++y){
                matrix_set(x,y,0);
            }
        }
    }
    
    if(button_clicked(RIGHT)) {
        for(uint8_t c = 110; c < 140; c = 10 + c){
            clear();            
            w_eins();
            wait_ms(c);
            clear();
            w_zwei();
            wait_ms(c);
            clear();
            w_drei();
            wait_ms(c);
            clear();
            w_vier();
            wait_ms(c);
            clear();
            w_funf();
            wait_ms(c);
            clear();
            w_sechs();
            wait_ms(c);
        }
        randm = rand()%6;
        clear();
    }
    if(randm==0){
        w_eins();
    }
    else if(randm==1){
        w_zwei();
    }
    else if(randm==2){
        w_drei();
    }
    else if(randm==3){
        w_vier();
    }
    else if(randm==4){
        w_funf();
    }
    else if(randm==5){
        w_sechs();
    }
}

REGISTER(wuerfel, init, NULL);
