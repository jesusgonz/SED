#include <16f876.h>
#fuses NOWDT
#use delay ( clock = 4000000)
#use standard_io(a)
#use standard_io(b)

int16 high_time = 0;
int16 low_time = 0;
int16 periodo = 0;
int1 estado = 0;
const int LOAD = 60;

/* CARGA INICIAL PARA MEDIR 50 ms = 60 */ 
#INT_EXT
void rsiext(){
	if(estado == 0)
		high_time = get_timer0();
		set_timer0(LOAD);
		estado = !estado;
		ext_int_edge(H_TO_L);
	else {
		low_time = get_timer0();
		set_timer0(LOAD);
		estado = !estado;
		ext_int_edge(L_TO_H);
	}
	
	periodo = high_time + low_time;
}

void generasenal(){
	output_high(PIN_A0);
	delay_ms(high_time/2);
	output_low(PIN_A0);
	delay_ms(low_time);
}


void main(){
	setup_timer_0(RTCC_INTERNAL | RTC_DIV_256);
	enable_interrupts(INT_EXT);
	ext_int_edge(L_TO_H);
	enable_interrupts(GLOBAL);

	while(1){
		generasenal();
	}
}
	
	


	
