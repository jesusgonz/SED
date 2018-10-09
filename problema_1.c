#include <16f876.h>
#fuses NOWDT
#use standard_io(a)
#use standard_io(b)
 
int desbordamientos = 0;
int16 low_time = 0;
int16 high_time = 0;
int1 estado = 0; /* Estado = 0, llega el pulso */ /*Estado = 1, el pulso está en alta */

#INT_TIMER0
void rsitimer0(){
	if(++desbordamientos < 8){
		if(estado == 0)
			high_time += get_timer0();
		else
			low_time += get_timer0();
	} else {
		desbordamientos = 0;
		high_time = 0;
		low_time = 0;
	}
}

#INT_EXT
void rsiext(){
	if(estado == 0){
		set_timer0(0);
		estado = !estado;
		ext_int_edge(H_TO_L);
	} else {
		set_timer0(0);
		estado = !estado;
		ext_int_edge(L_TO_H);
	}
	
	if(high_time > low_time)
		output_high(PIN_A0);
	else
		output_low(PIN_A0);
}

/* La tarea del main es configurar TIMER0 para que mida señales de un período máximo de 0.5 segundos */ 
/* TIMER0 solo puede contar como máximo 0.0625 segundos, necesito 8 desbordamientos de TIMER0 para medir 0.5 segundos */


void main(){
	setup_timer_0(RTCC_INTERNAL | RTC_DIV_256);
	enable_interrupts(INT_TIMER0);
	enable_interrupts(INT_EXT);
	ext_int_edge(L_TO_H);
	enable_interrupts(GLOBAL);

	while(1){}
}




	
