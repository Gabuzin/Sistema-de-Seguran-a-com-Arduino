#include <string.h>

// variaveis
unsigned int tempo=0,pressionado=0,res=10,resultado=0;
unsigned char tempoalarme=15,tempo_confirma=20,tempo_bemvindo=2;
unsigned char  estado=0,est=0,i=0,cont=0,retorno=1;	//declara variavel global
char senha[]= {"1000000"}, cod_risc[]={"1111111"}, cod_ok[]={"0000000"};
char confirma[]={"1111000"};
 unsigned char digitos[tam_vetor];
 float potmv=0,pot=0;

///  funcoes da maquina de lavar

//descricao dos pinos I/O
// ENTRADAS
#define botao_menos PC0 // botao menos
#define botao_mais PC1 // botão emergencia (vermelho/avança)
#define botao_enter PC2 // botao ENTER / STOP processo
#define sensor_mov PC3 // sensor de movimento (preto)
#define sensor_cerca PC4 //cerca eletrica (amarelo)

//SAIDAS
#define alarme PB2
#define liberado PB1 
#define cerca_eletrica PB3 


//defini�ao para acessar a mem�ria flash 
PROGMEM const char msg_inicio[] = "INICIANDO     \0"; //mensagem armazenada na memoria Flash
PROGMEM const char msg_inicio2[] = " SISTEMA     \0"; //mensagem armazenada na memoria Flash
PROGMEM const char msg_config[] = " MODO     \0";
PROGMEM const char msg_senha[] = "ALTERAR SENHA ?    \0";
PROGMEM const char msg_senha2[] = " SENHA     \0";
PROGMEM const char msg_config2[] = " CONFIGURACAO     \0";
PROGMEM const char msg_confirma[] = "prosseguir ?       \0";
PROGMEM const char msg_cancela[] = "NAO       \0";
PROGMEM const char msg_avanca[] = "SIM        \0";
PROGMEM const char msg_tempoalarme[] = "Tempo Alarme       \0";
PROGMEM const char msg_reiniciando[] = "Reiniciando....  \0"; //mensagem armazenada na memoria Flash
PROGMEM const char msg_definecodigo_ok[] = "Codigo Seguro       \0";
PROGMEM const char msg_codigo[] = "ALTERAR CONFIRMACAO ?    \0";
PROGMEM const char msg_definecodigo_risc[] = "Codigo Perigo       \0";
PROGMEM const char msg_redefine[]= "REDEF PARAMETROS ?    \0";
PROGMEM const char msg_iniciando[]=  "INICIAR SISTEMA ?    \0";
PROGMEM const char msg_monitoramento[]= "SISTEMA ATIVO     \0";
PROGMEM const char msg_aguardo[]= "Insira a senha     \0";
PROGMEM const char msg_bemvindo[] = "Bem vindo !!!            \0";
void inicio();
void configuracao();
void nova_senha();
void tempo_alarme();
void altera_senha(); 
void novo_codigo();
void tempo_confirmacodigo();
void define_codigo_ok();
void define_codigo_risc();
void redefine_parametros();
void monitoramento();
void insere_senha();
void aguarda_senha();
void alarme_ativo();
void bem_vindo();
void confirmacao();
void policia();

// configura  AD e PWM

void configura_AD()
{
	// configura canal
	ADMUX = (1<<REFS0) | (1<<MUX2) | (1<<MUX0);			//Tens�o interna de ref (+5V), canal ADC5
	ADCSRA = (1<<ADEN) | (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);	//habilita o AD e define um prescaler de 128 (clk_AD = F_CPU/128), 125 kHz
}

void set_pwm_pb3(unsigned int valor)  // timer 2
{
	TCCR2A |=  (1<< COM2A1) | (1 << WGM21) | (1<<WGM20);
	TCCR2B =  0b00000111;
	OCR2A=valor;
	sei();
}

signed int le_pot()
{
	set_bit(ADCSRA, ADSC);								//inicia a convers�o
	while(tst_bit(ADCSRA,ADSC));						//espera a convers�o ser finalizada
	
	return (ADC);
}


//----------------------------- funções de programa ------------------------------------------


void inicio()
{

	cmd_LCD(0x83,0);  //limpa display
	escreve_LCD_Flash(msg_inicio);  // imprime mensagem
	cmd_LCD(0xC3,0);  //limpa display
	escreve_LCD_Flash(msg_inicio2); // imprime mensagem

		if(!tempo && estado ==11) 
		{    // vefica tempo=0
			estado=11; 
			tempo=2; //muda de estado
			cmd_LCD(0x01,0);  // limpa display
		}
		if(!tempo && estado !=11) 
		{    // vefica tempo=0
			estado=1; 
			tempo=2; //muda de estado
			cmd_LCD(0x01,0);  // limpa display
		}


}

void configuracao()
{
  cmd_LCD(0x84,0);  //limpa display
	escreve_LCD_Flash(msg_config);  // imprime mensagem
	cmd_LCD(0xC1,0);  //limpa display
	escreve_LCD_Flash(msg_config2);  // imprime mensagem

		if(!tempo) 
		{    // vefica tempo=0
			estado=2; 
			tempo= 2; //muda de estado
			cmd_LCD(0x01,0);  // limpa display
		}


}
void nova_senha()
{

  cmd_LCD(0x80,0);  //limpa display
	escreve_LCD_Flash(msg_senha);  // imprime mensagem

	if(!tst_bit(PINC,0)) { cmd_LCD(0xC5,0);	//posiciona lcd
							escreve_LCD_Flash(msg_cancela);
							est = 0; } //if botao+
	if(!tst_bit(PINC,1)) { cmd_LCD(0xC5,0);	//posiciona lcd
							escreve_LCD_Flash(msg_avanca); 
							est = 1;} //if botao-
	if(!tst_bit(PINC,2) && est == 1) { estado=3;  _delay_ms(300); cmd_LCD(0x01,0);tempo=2;}    // enter
	if(!tst_bit(PINC,2) && est == 0) { estado=4; _delay_ms(300); cmd_LCD(0x01,0); }
		

}
void altera_senha()
{
   	cmd_LCD(0x80,0); 	
	escreve_LCD_Flash(msg_senha2); // imprime mensagem
	cmd_LCD(0xC0,0);	//posiciona lcd
	cmd_LCD(senha[0],1);
	cmd_LCD(senha[1],1);
	cmd_LCD(senha[2],1);
	cmd_LCD(senha[3],1);
	cmd_LCD(senha[4],1);
	cmd_LCD(senha[5],1);

	if(!tst_bit(PINC,0)) { cmd_LCD(0xC5,0);	//posiciona lcd
							cmd_LCD('0',1);
						 
							senha[i]='0';
							 } //if botao+
	if(!tst_bit(PINC,1)) { cmd_LCD(0xC5,0);	//posiciona lcd
						  cmd_LCD('1',1);
							senha[i]='1';
							} //if botao-
	if(!tst_bit(PINC,2) && i<5) { i++;  _delay_ms(300);}    // enter
	if(!tst_bit(PINC,2) && i == 5) { i=0;estado=4; cmd_LCD(0x01,0); _delay_ms(300); }
}

void tempo_alarme()
{
		cmd_LCD(0x80,0); 	
		escreve_LCD_Flash(msg_tempoalarme); // imprime mensagem
		ident_num(tempoalarme,digitos); // separa digitos
		cmd_LCD(0xC5,0);	//posiciona lcd
		cmd_LCD(digitos[2],1);
		cmd_LCD(digitos[1],1);
		cmd_LCD(digitos[0],1);
  	cmd_LCD(0xC9,0);
		cmd_LCD('<',1);
		cmd_LCD('E',1);
		cmd_LCD('N',1);
		cmd_LCD('T',1);
		cmd_LCD('E',1);
		cmd_LCD('R',1);
		cmd_LCD('>',1);
		if(!tst_bit(PINC,0)) { tempoalarme--; _delay_ms(100);} //if botao+
		if(!tst_bit(PINC,1)) { tempoalarme++; _delay_ms(100); } //if botao-
		if(!tst_bit(PINC,2)) { estado=5;  _delay_ms(300); cmd_LCD(0x01,0); } 
}


void novo_codigo()
{
  cmd_LCD(0x80,0);  //limpa display
  escreve_LCD_Flash(msg_codigo);  // imprime mensagem

	if(!tst_bit(PINC,0)) { cmd_LCD(0xC5,0);	//posiciona lcd
							escreve_LCD_Flash(msg_cancela);
							est = 0; } //if botao+
	if(!tst_bit(PINC,1)) { cmd_LCD(0xC5,0);	//posiciona lcd
							escreve_LCD_Flash(msg_avanca); 
							est = 1;} //if botao-
	if(!tst_bit(PINC,2) && est == 1) { estado=6;  _delay_ms(300); cmd_LCD(0x01,0);tempo=2;}    // enter
	if(!tst_bit(PINC,2) && est == 0) { estado=8; cmd_LCD(0x01,0);_delay_ms(300); }
		

}


void define_codigo_ok()
{
	cmd_LCD(0x80,0); 	
	escreve_LCD_Flash(msg_definecodigo_ok); // imprime mensagem
	cmd_LCD(0xC0,0);	//posiciona lcd
	cmd_LCD(cod_ok[0],1);
	cmd_LCD(cod_ok[1],1);
	cmd_LCD(cod_ok[2],1);
	cmd_LCD(cod_ok[3],1);
	cmd_LCD(cod_ok[4],1);
	cmd_LCD(cod_ok[5],1);

	if(!tst_bit(PINC,0)) { cmd_LCD(0xC5,0);	//posiciona lcd
							cmd_LCD('0',1);
							cod_ok[i]='0';
							 } //if botao+
	if(!tst_bit(PINC,1)) { cmd_LCD(0xC5,0);	//posiciona lcd
							cmd_LCD('1',1);
							cod_ok[i]='1';
							} //if botao-
	if(!tst_bit(PINC,2) && i<5) { i++;  _delay_ms(300);}    // enter
	if(!tst_bit(PINC,2) && i == 5) { i=0; estado=7; cmd_LCD(0x01,0); _delay_ms(300); }
	

}

void define_codigo_risc()
{
		cmd_LCD(0x80,0); 	
	escreve_LCD_Flash(msg_definecodigo_risc); // imprime mensagem
	cmd_LCD(0xC0,0);	//posiciona lcd
	cmd_LCD(cod_risc[0],1);
	cmd_LCD(cod_risc[1],1);
	cmd_LCD(cod_risc[2],1);
	cmd_LCD(cod_risc[3],1);
	cmd_LCD(cod_risc[4],1);
	cmd_LCD(cod_risc[5],1);

	if(!tst_bit(PINC,0)) { cmd_LCD(0xC5,0);	//posiciona lcd
							cmd_LCD('0',1);
							cod_risc[i]='0';
							 } //if botao+
	if(!tst_bit(PINC,1)) { cmd_LCD(0xC5,0);	//posiciona lcd
							cmd_LCD('1',1);
							cod_risc[i]='1';
							} //if botao-
	if(!tst_bit(PINC,2) && i<5) { i++;  _delay_ms(300);}    // enter
	if(!tst_bit(PINC,2) && i == 5) { i=0; estado=8; cmd_LCD(0x01,0); _delay_ms(300); }
	

}

void tempo_confirmacodigo()
{
		cmd_LCD(0x80,0); 	
		escreve_LCD_Flash(msg_tempoalarme); // imprime mensagem
		ident_num(tempo_confirma,digitos); // separa digitos
		cmd_LCD(0xC5,0);	//posiciona lcd
		cmd_LCD(digitos[2],1);
		cmd_LCD(digitos[1],1);
		cmd_LCD(digitos[0],1);
  	cmd_LCD(0xC9,0);
		cmd_LCD('<',1);
		cmd_LCD('E',1);
		cmd_LCD('N',1);
		cmd_LCD('T',1);
		cmd_LCD('E',1);
		cmd_LCD('R',1);
		cmd_LCD('>',1);
		if(!tst_bit(PINC,0)) { tempo_confirma = tempo_confirma-10; _delay_ms(100);} //if botao+
		if(!tst_bit(PINC,1)) { tempo_confirma = tempo_confirma+10; _delay_ms(100); } //if botao-
		if(!tst_bit(PINC,2)) { estado=9;  _delay_ms(300); cmd_LCD(0x01,0); } 
}

void define_pot()
{ 
	cmd_LCD(0x80,0); 	
	escreve_LCD("Potencia cerca"); // imprime mensagem
	pot=le_pot();
	
	
	
	set_pwm_pb3((int)pot/4);
	potmv= (pot/1023)*100;

	
	ident_num(potmv,digitos);
	cmd_LCD(0xC0,0);
	
	cmd_LCD(digitos[2],1);
	cmd_LCD(digitos[1],1);
	cmd_LCD(digitos[0],1);
	cmd_LCD('%',1);
	_delay_ms(100);	
	if(!tst_bit(PINC,2)) { estado=10;  _delay_ms(300); cmd_LCD(0x01,0);tempo=2;set_pwm_pb3(0);}    // enter	
}


void inicio_sistema()
{
	cmd_LCD(0x80,0); 	
	escreve_LCD_Flash(msg_iniciando); 

  if(!tst_bit(PINC,0)) { cmd_LCD(0xC5,0);	//posiciona lcd
							escreve_LCD_Flash(msg_cancela);
							est = 0; } //if botao+
	if(!tst_bit(PINC,1)) { cmd_LCD(0xC5,0);	//posiciona lcd
							escreve_LCD_Flash(msg_avanca); 
							est = 1;} //if botao-
	if(!tst_bit(PINC,2) && est == 1) { estado=12;  _delay_ms(300); cmd_LCD(0x01,0);tempo=2;est=0;}    // enter
	if(!tst_bit(PINC,2) && est == 0) { estado=2; _delay_ms(300); cmd_LCD(0x01,0); tempo=2;est=0;}
}

void monitoramento()
{
	cmd_LCD(0x80,0); 	
	escreve_LCD_Flash(msg_monitoramento); 
	set_pwm_pb3(pot);      
        if(!tst_bit(PINC,sensor_mov) || !tst_bit(PINC,sensor_cerca))                     // testa sensores
	        {     cmd_LCD(0x01,0);           
                cmd_LCD(0x80,0);
								tempo=tempoalarme;                  // 
                escreve_LCD_Flash(msg_aguardo);
							
								estado= 13;  // imprime mensagem
	    	  }

}

void insere_senha()
{
  ident_num(tempo,digitos);  //divide por 10 antes de separar digitos					
	cmd_LCD(0xC9,0);
	cmd_LCD(digitos[2],1); //
	cmd_LCD(digitos[1],1);
	cmd_LCD(digitos[0],1);

	escreve_LCD_Flash(msg_senha2); // imprime mensagem
	cmd_LCD(0xC0,0);	//posiciona lcd
	cmd_LCD(confirma[0],1);
	cmd_LCD(confirma[1],1);
	cmd_LCD(confirma[2],1);
	cmd_LCD(confirma[3],1);
	cmd_LCD(confirma[4],1);
	cmd_LCD(confirma[5],1);
	cmd_LCD(confirma[6],1);

	if(!tst_bit(PINC,0)) { cmd_LCD(0xC5,0);	//posiciona lcd
		
						confirma[i]='0';
				//		_delay_ms(300);
						 } //if botao+
	if(!tst_bit(PINC,1)) { cmd_LCD(0xC5,0);	//posiciona lcd
				
						confirma[i]='1';
					//	_delay_ms(300);
						} //if botao-
	
	if(!tst_bit(PINC,2) && i<6) 
	{ 
		
		i++;  	
		set_bit(PORTB,liberado);
		_delay_ms(300);
		clr_bit(PORTB,liberado);
	}    // enter
	
	if(i==6)
	{
		cpl_bit(PORTB,alarme);
		_delay_ms(100);
		cpl_bit(PORTB,alarme);
		
	}
	if(!tst_bit(PINC,2) && i == 6 && strcmp(confirma,senha)==0) 
		{
			i=0;
			cmd_LCD(0x01,0);
			tempo=tempo_bemvindo;
			estado=15;
			_delay_ms(300);
		
		}
  
	else if(!tempo)
	{
		tempo=tempo_confirma;
		estado=14;	
		i=0;
		
		cmd_LCD(0x01,0);
	}

	
	if(!tst_bit(PINC,2) && i == 6 && strcmp(confirma,senha)!=0)
	{
		cont++;
		i=0;
		tempo=tempo_confirma;
		confirma[0]='1';
		confirma[1]='1';
		confirma[2]='1';
		confirma[3]='1';
		confirma[4]='0';
		confirma[5]='0';
		confirma[6]='0';
		_delay_ms(300);
		if(cont == 2)
		{ estado=14;cmd_LCD(0x01,0);}
		
	}


}
void bem_vindo()
{
	set_bit(PORTB,liberado);  
	cmd_LCD(0x80,0); 	
	escreve_LCD_Flash(msg_bemvindo);
	clr_bit(PORTB,alarme);
	if(!tempo)
	{
		clr_bit(PORTB,liberado);
		
		i=0;
		confirma[0]='1';
		confirma[1]='1';
		confirma[2]='1';
		confirma[3]='1';
		confirma[4]='0';
		confirma[5]='0';
		confirma[6]='0';
		
		tempo=tempo_confirma;
		estado=10;
	}	
}


void alarme_ativo()
{
  cont=0;
  
  
  cmd_LCD(0x80,0);
  escreve_LCD("Alarme disparado");
  set_bit(PORTB,alarme);  
  


   escreve_LCD_Flash(msg_senha2); // imprime mensagem
   cmd_LCD(0xC0,0);	//posiciona lcd
   cmd_LCD(confirma[0],1);
   cmd_LCD(confirma[1],1);
   cmd_LCD(confirma[2],1);
   cmd_LCD(confirma[3],1);
   cmd_LCD(confirma[4],1);
   cmd_LCD(confirma[5],1);
   cmd_LCD(confirma[6],1);

   if(!tst_bit(PINC,0)) { cmd_LCD(0xC5,0);	//posiciona lcd
	   
	   confirma[i]='0';
	
   } //if botao+
   
   if(!tst_bit(PINC,1)) { cmd_LCD(0xC5,0);	//posiciona lcd
	   
	   confirma[i]='1';
	 
   } //if botao-
   
   if(!tst_bit(PINC,2) && i<6)
   {
	   i++;
	   set_bit(PORTB,liberado);
	   _delay_ms(300);
	   clr_bit(PORTB,liberado);
   }    // enter
    
   
   if(!tst_bit(PINC,2) && i == 6 && strcmp(confirma,senha)==0)
   {
	   i=0;
	   cmd_LCD(0x01,0);
	   _delay_ms(300);
	   estado=16;
	   
   }    
   
   if(!tst_bit(PINC,2) && i == 6 && strcmp(confirma,senha)!=0)
   {  
	   i=0;
	   confirma[0]='1';
	   confirma[1]='1';
	   confirma[2]='1';
	   confirma[3]='1';
	   confirma[4]='0';
	   confirma[5]='0';
	   confirma[6]='0';
	   _delay_ms(300);
   
   }  
   if(!tempo)
   {
	   cmd_LCD(0x01,0);
	   estado=17;
   }
   
   //tempo se não chama a policia
}

void confirmacao()
{
	cmd_LCD(0x80,0);
	escreve_LCD("COD CONFIRMACAO");
	
	  escreve_LCD_Flash(msg_senha2); // imprime mensagem
	  cmd_LCD(0xC0,0);	//posiciona lcd
	  cmd_LCD(confirma[0],1);
	  cmd_LCD(confirma[1],1);
	  cmd_LCD(confirma[2],1);
	  cmd_LCD(confirma[3],1);
	  cmd_LCD(confirma[4],1);
	  cmd_LCD(confirma[5],1);
	  cmd_LCD(confirma[6],1);

	  if(!tst_bit(PINC,0)) { cmd_LCD(0xC5,0);	//posiciona lcd
		  
		  confirma[i]='0';
		//  _delay_ms(100);
	  } //if botao+
	  
	  if(!tst_bit(PINC,1)) { cmd_LCD(0xC5,0);	//posiciona lcd
		  
		  confirma[i]='1';
		//  _delay_ms(100);
	  } //if botao-
	  
	  if(!tst_bit(PINC,2) && i<6)
	  {
	
		  i++;
		  set_bit(PORTB,liberado);
		  _delay_ms(300);
		  clr_bit(PORTB,liberado);
		
		
	  }    // enter
	  
	  
	   if(!tst_bit(PINC,2) && i == 6 && strcmp(confirma,cod_ok)==0)
	   {
		   i=0;
		   cmd_LCD(0x01,0);
		   tempo=tempo_bemvindo;
		   estado=15;
		   
	   }
	   
	   if(!tst_bit(PINC,2) && i == 6 && strcmp(confirma,cod_risc)==0)
	   {
		   i=0;
		  cmd_LCD(0x01,0);
		  estado = 17;  
	   }
	   
	      if(!tst_bit(PINC,2) && i == 6 && strcmp(confirma,cod_risc)!=0 && strcmp(confirma,cod_ok)!=0)
   {  
	   i=0;
	  // cont++;
	   confirma[0]='1';
	   confirma[1]='1';
	   confirma[2]='1';
	   confirma[3]='1';
	   confirma[4]='0';
	   confirma[5]='0';
	   confirma[6]='0';
      _delay_ms(300);
   }  
	  
}

void policia()
{
	cmd_LCD(0x80,0);
	escreve_LCD("Chamando");
	cmd_LCD(0xC0,0);
	escreve_LCD("Autoridades");
	
}



void reinicializando()
    {
			_delay_ms(500); 
    	estado = 1;
    }


