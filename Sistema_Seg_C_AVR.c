
//=====================================================================================	//
//		SISTEMA DE SEGURANCA UTILIZANDO FUNCOES DE MENU    							    //
//																						//
//		LCD Interface de dados de 4 bits												//
//      Adaptacao: Gabriel Ditomaso   versao: AVR_C abril/2023                          //
//=====================================================================================	//

#include "def_principais.h"			//inclusao do arquivo com as principais definicoes
#include "LCD.h"   // biblioteca .c e .h LCD display 16x2
#include "funcoes_Sistema.h"

void set_pwm_pb3(unsigned int valor);
signed int le_pot();

ISR(TIMER1_OVF_vect)                              //interrupção do TIMER1 
{
  TCNT1 = 0xC2F7;    // Renicia TIMER
	if(tempo)
	{tempo--;}    // subtrai tempo se diferente de 0
}



//-----------------------------------------------------------------------------------------
int main()
{	
	
 	//declaracao da vari�vel para armazenagem dos digitos
 // int est = 0 ;
	DDRB = 0b00111111;			//PORT B saida
	PORTB= 0;		        	//inicia desligado
	DDRC = 0b00000000;			//PORT C entrada
    PORTC= 0b11111111;          //PULL UP portC
	DDRD = 0xFF;				//PORTD como sa�da (display)
	PORTD= 0xFF;				//desliga o display
	UCSR0B = 0x00;				//PD0 e PD1 como I/O gen�rico, para uso no Arduino
	DDRC= 0b00000000;           // saidas motor e valvulas
	
// Configuração do timer1  ----------------------------------------------------------------
    
  TCCR1A = 0;                        //confira timer para operação normal pinos OC1A e OC1B desconectados
  TCCR1B = 0;                        //limpa registrador
  TCCR1B |= (1<<CS10)|(1 << CS12);   // configura prescaler para 1024: CS12 = 1 e CS10 = 1
 
  TCNT1 = 0xC2F7;                    // incia timer com valor para que estouro ocorra em 1 segundo
                                     // 65536-(16MHz/1024/1Hz) = 49911 = 0xC2F7
  
  TIMSK1 |= (1 << TOIE1);           // habilita a interrupção do TIMER1 por overflow
	sei();

	//--------------------------------------------------------------------------------------
	
	inic_LCD_4bits();				//inicializa o LCD
	configura_AD();
	cmd_LCD(0x01,0); 				//desloca cursor para a segunda linha
	
	tempo=2;   // varial de contagem de tempo
	while(1) 					//laco infinito
	{
		
		switch(estado)
		{  //estado inicial
		    	case 0: inicio(); break; 
			case 1: configuracao(); break; 
			case 2: nova_senha(); break;
			case 3: altera_senha(); break;
			case 4: tempo_alarme(); break;
			case 5: novo_codigo(); break;
			case 6: define_codigo_ok(); break;
			case 7: define_codigo_risc(); break;
			case 8: tempo_confirmacodigo(); break;
			case 9: define_pot(); break; 
			case 10: inicio_sistema(); break; 
			case 11: inicio(); break;
			case 12: monitoramento(); break;
			case 13: insere_senha(); break;
			case 14: alarme_ativo(); break;
			case 15: bem_vindo(); break;
			case 16: confirmacao(); break;
			case 17: policia(); break;
				//desligar alarme
				// codigo de confirmação
				

			
		}
		
		if(estado>5)  // testes de estados emergencia tilizando botao ENTER a partir do estado 5
		{
			if(!tst_bit(PINC,2))  // verifica se botao enter pressionado
			{ pressionado++;  // incrementa variavel pressionado
				if(pressionado>3000)  // compara se ultrapassou o valor limit, botao pressionado por longo periodo
				{ estado=15; }   // direciona para estado 14
			} else
			{
				pressionado=0;  // botao nao pressionado  zera valor
			}  // fim if botao pressionado
		} // fim if teste de estados emergencia
		
	}// fim do while
	
} // fim do main
//======================================================================================


