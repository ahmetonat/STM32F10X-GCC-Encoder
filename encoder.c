


#include "encoder.h"


//Retains the state of the keys for the encoder.
static __IO uint8_t keystate[NumKeys];

void init_ENC (void){
  int i;
  init_ENC_GPIO();
  //TODO: Prepare a new init_ENC(), and place this into that function.
  //      Call init_ENC_GPIO from there.
  for (i=0; i<NumKeys;++i){
    keystate[i]=KeyAtRest;
    //    keyoldstate[i]=keystate[i];
  }
}

//Fixed GPIO initialization code.
void init_ENC_GPIO (void){

  GPIO_InitTypeDef GPIO_InitStruct;
  
  // Enable GPIOA (ENC) Clock
  RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE ); 
  
  GPIO_StructInit (&GPIO_InitStruct);

  // Configure ENC pins
  GPIO_InitStruct.GPIO_Pin = ENC_A_GPIO_PIN| ENC_B_GPIO_PIN| ENC_C_GPIO_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; //Input pull up.
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(ENC_GPIO_PORT, &GPIO_InitStruct);



}


//************* Functions *******************************************
void check_keypress(uint8_t key){
  // Checks the button passed in the argument, and
  //  debounces it using a digital LPF.
  // The decision is returned as the corresponding element of 
  //  of the variable keystate[] (==KeyPressed or KeyAtRest)
  
  int x;
  int tmp;
  int keyread;
  //  static __IO uint8_t key1dsp;
  static uint8_t keydsp[NumKeys]; //Initialized to zero by compiler.

  //This part depends on the hardware:
  if (key==ENC_A){
    keyread=GPIO_ReadInputDataBit (ENC_GPIO_PORT, ENC_A_GPIO_PIN);
  }else if (key==ENC_B){
    keyread=GPIO_ReadInputDataBit (ENC_GPIO_PORT, ENC_B_GPIO_PIN);
  }else if (key==ENC_C){
    keyread=GPIO_ReadInputDataBit (ENC_GPIO_PORT, ENC_C_GPIO_PIN);
  }
   if (keyread==KeyPressed){
     x=0x3F;
       }else{ //KeyAtIdle;
     x=0;
   }
   // The LPF code is:
   //y(t) = 0.25x(t)+0.75y(t)  DSP based debounce.
   //1.0=FF, 0.25=0x3F, 0.75y(t)= y(t)-(y(t)>>2)

   tmp=(keydsp[key] >>2); //=key1state/4
   keydsp[key]= x+keydsp[key]-tmp; //keydsp*3/4

   //hysteresis:
   if ((keydsp[key]>0xF0) && (keystate[key]==KeyAtRest)){
       keystate[key]=KeyPressed;
     } //else 
     if ((keydsp[key] <0x0F) && (keystate[key]==KeyPressed)){
       keystate[key]=KeyAtRest;
     }
}



int EncRead_Guarded(void){
  int retval;
  static int keyoldstate[NumKeys]; //Static to make it fast...

      //If ENC_A is pressed:
      keyoldstate[ENC_A]=keystate[ENC_A]; 
      check_keypress(ENC_A);
      keyoldstate[ENC_B]=keystate[ENC_B]; 
      check_keypress(ENC_B);
      keyoldstate[ENC_C]=keystate[ENC_C]; 
      check_keypress(ENC_C);

      retval=NOCHANGE;
      
      //      if(keyoldstate[ENC_C]!=keystate[ENC_C]){
	if (keystate[ENC_C]==KeyPressed){
	  retval = CLICK;  //Signal caller that encoder was pressed.
	  if(keyoldstate[ENC_A]!=keystate[ENC_A]){
	    if (keystate[ENC_A]==KeyPressed){ //Rising edge on A
	      //xprintf ("A+ ");
	      if(keystate[ENC_B]==KeyPressed){
		//xprintf ("DN ");
		retval = DOWN;
	      }else{
		//xprintf ("UP ");
		retval=UP;
	      }
	    }else if(keystate[ENC_A]==KeyAtRest) {//Falling edge on A
	      //xprintf ("A- ");
              if(keystate[ENC_B]==KeyAtRest){
                //xprintf ("DN ");
                retval = DOWN;
              }else{
                //xprintf ("UP ");
                retval=UP;
              }
	    }   // END: else if(keystate[ENC_A]==KeyAtRest)
	  }	// END: if(keyoldstate[ENC_A]!=keystate[ENC_A])
	}       // END: if (keystate[ENC_C]==KeyPressed)

        /*  //The code below might be useful for debug.
      // If ENC_B is pressed:
      if(keyoldstate[ENC_B]!=keystate[ENC_B]){
	if (keystate[ENC_B]==KeyPressed){
	  //xprintf ("B+ ");
	}else{
	  //xprintf ("B- ");
	}
      }	//END: if ENC_B

      // If ENC_C is pressed:
      if(keyoldstate[ENC_C]!=keystate[ENC_C]){
	if (keystate[ENC_C]==KeyPressed){
	  //LED_ON();
	  //xprintf ("C+ ");
	}else{
	  //LED_OFF();
	  //xprintf ("C- ");
	}
      }	//END: if ENC_C
	*/
      return (retval);
}
