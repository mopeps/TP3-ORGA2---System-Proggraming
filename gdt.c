/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de la tabla de descriptores globales
*/

#include "gdt.h"
#include "tss.h"
//#define GDT_CODIGO_0 10
//#define GDT_DATO_0 11
//#define GDT_CODIGO_3 12
//#define GDT_DATO_3 13
//#define SEGMENTO_PANTALLA 14

gdt_entry_t gdt[GDT_COUNT] = {
    /* Descriptor nulo*/
    /* Offset = 0x00 */
    [GDT_IDX_NULL_DESC] =  {
            .limit_15_0 = 0x0000,
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .type = 0x0,
            .s = 0x00,
            .dpl = 0x00,
            .p = 0x00,
            .limit_19_16 = 0x00,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x0,
            .g = 0x00,
            .base_31_24 = 0x00,
        },
    [GDT_IDX_CODIGO_0] =  {
            .limit_15_0 = 0xC900,
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .type = 0x08,
            .s = 0x01,      //Code-Data Segment
            .dpl = 0x00,
            .p = 0x01,
            .limit_19_16 = 0x00,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x1,
            .g = 0x01,
            .base_31_24 = 0x00,
        },

    [GDT_IDX_CODIGO_3] =  {
            .limit_15_0 = 0xC900,
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .type = 0x8,// 0x8,
            .s = 0x01,
            .dpl = 0x03,
            .p = 0x01,
            .limit_19_16 = 0x00,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x1,
            .g = 0x01,
            .base_31_24 = 0x00,
        },

    [GDT_IDX_DATO_0] = {
            .limit_15_0 = 0xC900,
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .type = 0x2,      //Read/Write
            .s = 0x01,
            .dpl = 0x00,
            .p = 0x01,
            .limit_19_16 = 0x00,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x1,
            .g = 0x01,
            .base_31_24 = 0x00,
        },
    [GDT_IDX_DATO_3] =    {
            .limit_15_0 = 0xC900,
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .type = 0x2,       //Read/Write
            .s = 0x01,
            .dpl = 0x03,
            .p = 0x01,
            .limit_19_16 = 0x00,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x1,
            .g = 0x01,
            .base_31_24 = 0x00,
        },        
     [SEGMENTO_IDX_PANTALLA] =    {
            .limit_15_0 = 0x1F40,
            .base_15_0 = 0x8000,
            .base_23_16 = 0x0B,
            .type = 0x2,       //Read/Write
            .s = 0x01,
            .dpl = 0x00,
            .p = 0x01,
            .limit_19_16 = 0x00,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x1,
            .g = 0x00,
            .base_31_24 = 0x00,
        },

     [GDT_IDX_TSS_INIT] =  {
            .limit_15_0 = sizeof(tss_t)-1,
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .type = 0x9,
            .s = 0x00,
            .dpl = 0x00, 
            .p = 0x01,
            .limit_19_16 = 0x00,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x1,
            .g = 0x00,
            .base_31_24 = 0x00,
        },
      
     [GDT_IDX_TSS_IDLE] =  {
            .limit_15_0 = sizeof(tss_t)-1,
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .type = 0x9,
            .s = 0x00,
            .dpl = 0x00, 
            .p = 0x01,
            .limit_19_16 = 0x00,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x1,
            .g = 0x00,
            .base_31_24 = 0x00,
        },
};

gdt_descriptor_t GDT_DESC = {sizeof(gdt) + 48, (uint32_t)&gdt};
