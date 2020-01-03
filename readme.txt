/**
  @page exampletemplate Project Example Template
  
  @verbatim
  ******************** (C) COPYRIGHT 2014 STMicroelectronics *******************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @version V1.6.1
  * @date    30-September-2014
  * @brief   This sub-directory contains all the user-modifiable files 
  *          needed to create a new project linked with the STM8L15x 
  *          Standard Peripheral Library.
  ******************************************************************************
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
   @endverbatim

@par Project Example Template Description 

This example is used as a template that can be used as reference to build any new 
firmware development for all STM8L product devices using the STM8L15x/16x/05x/AL3Lx/AL31x 
Standard Peripherals Library.


@par Directory contents 

This folder contains standard template projects for EWSTM8, STVD with Cosmic, 
STVD with raisonance and RIDE toolchains that compile the needed STM8L Standard
Peripheral's drivers plus all the user-modifiable files that are necessary to 
create a new project. 

  - Project/STM8L15x_StdPeriph_Template/stm8l15x_conf.h    Library Configuration file

  - Project/STM8L15x_StdPeriph_Template/stm8l15x.c:     Source file containing the interrupt handlers (the 
                                                        function bodies are empty in this template).

  - Project/STM8L15x_StdPeriph_Template/stm8l15x.h      Header file including all interrupt handlers prototypes.

  - Project/STM8L15x_StdPeriph_Template/main.c          Main program
          
 @note
  - Low-Density devices are STM8L15x microcontrollers where the Flash memory 
    density ranges between 4 and 8 Kbytes.
  - Medium-Density devices are STM8L15x microcontrollers where the 
    Flash memory density ranges between 16 and 32 Kbytes.
  - Medium density Plus devices are STM8L151R6, STM8L152R6 microcontrollers 
    where the Flash memory density is fixed and equal to 32 Kbytes and with 
    wider range of peripheral and features than the medium density devices. 
  - High-Density devices are STM8L15x microcontrollers where the 
    Flash memory density is 64 Kbytes and with the same peripheral set than 
    Medium Density Plus devices.
  - Value line low density STM8L05xx devices are STM8L051x3 microcontrollers
    with 8-KB Flash
  - Value line medium density STM8L05xx devices are STM8L052x6 microcontrollers 
    with 32-KB Flash
  - Value line high density STM8L05xx devices: STM8L052x8 microcontrollers
    with 64-KB Flash
  - Medium density STM8AL31xx/STM8AL3Lxx devices: STM8AL3168, STM8AL3166, 
    STM8AL3148,STM8AL3146, STM8AL3138, STM8AL3136, STM8AL3L68, STM8AL3L66, 
    STM8AL3L48, STM8AL3L46 microcontrollers with 8-KB, 16-KB or 32-KB Flash
    
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
