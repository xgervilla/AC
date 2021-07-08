#include "CacheSim.h"
#include <stdio.h>
/* Posa aqui les teves estructures de dades globals
 * per mantenir la informacio necesaria de la cache
 * */

#define num_lineas 128 //2^2 * 2^10 (4Kb tam. cache) / 2^5 (32bytes x l’nea) = 2^7 l’neas -> 7 bits para linea_mc
#define offset 5    //2^5 (32bytes x l’nea) -> 5 bits para byte
#define tag_desp 12      //desplazamiento para quedarnos con el tag (5 de offset+7 de l’nea_mc)
/*
 *tag = address<0-19>
 *linea_mc = address<20-26>
 *bloque_m = address<0-26>
 *byte = address<27-31>
 
como es Write Through + NO Allocate:
    -en caso de fallo o acierto de escritura no se trae el bloque a cachŽ as’ que escribe directamente en memoria principal (1 byte)
    -en caso de fallo de lectura se trae el bloque a cachŽ (32 bytes)
    -en caso de acierto de lectura no se hace nada
*/

int hits, misses;
int bytes_accessed,mem_access;

typedef struct{
    unsigned int valido;
    unsigned int tag;
} linea_cache;


linea_cache cache[num_lineas];
/* La rutina init_cache es cridada pel programa principal per
 * inicialitzar la cache.
 * La cache es inicialitzada al començar cada un dels tests.
 * */
void init_cache (){
    hits = misses = 0;
    bytes_accessed = 0;
    for(int i = 0; i<num_lineas; ++i){
        //preparamos la estructura de la cachŽ (dejamos todas las l’neas como no v‡lidas -> vac’a)
        cache[i].valido = 0;
    }
}

/* La rutina reference es cridada per cada referencia a simular */
void reference (unsigned int address, unsigned int LE){
    unsigned int byte;
    unsigned int bloque_m;
    unsigned int linea_mc;
    unsigned int tag;
    unsigned int miss;      //hay fallo al acceder a la linea de cachŽ
    unsigned int lec_mp;    //leemos de memoria principal
    unsigned int mida_lec_mp;   //tama–o de lo que leemos en memoria principal (si leemos)
    unsigned int esc_mp;    //escribimos en memoria principal
    unsigned int mida_esc_mp;   //tama–o de lo que escribimos en memoria principal
    unsigned int replacement;   //reemplazamos linea de cachŽ
    unsigned int tag_out;   //tag de la linea que reemplazamos
    
    //separamos la direccion en los valores que interesan
    tag = address >> tag_desp; //20 bits (32-12)
    bloque_m = address >> offset; //27 bits
    linea_mc = (address & 0x00000FE0) >> offset; //7 bits
    byte = address & 0x0000001F; //5 bits
    
    //dejamos los valores de acceso a memoria principal en 0, si hay que acceder ya se modificar‡n
    lec_mp = esc_mp = mida_esc_mp = mida_lec_mp = 0;
    
    //comprobamos si la linea est‡ disponible
    if(cache[linea_mc].valido){
        //comprobamos si coincide el tag (hay hit)
        if(cache[linea_mc].tag == tag){
            //hay hit
            ++hits;
            replacement = 0;
            miss = 0;
            
            //comprobamos si es escritura (si es lectura no hace falta modificar nada m‡s)
            if (LE){
                esc_mp = 1; //hay que escribir en memoria principal;
                mida_esc_mp = 1; //solo escribimos 1 byte
                ++bytes_accessed;
                ++mem_access;
            }
        }
        //si el tag no coincide hay miss
        else{
            ++misses;
            miss = 1;
            //hay que ver si se trata de un acceso de lectura o de escritura
            if(LE){
                //acceso de escritura
                replacement = 0; //no hace falta reemplazar
                esc_mp = 1; //hay que escribir en memoria principal
                mida_esc_mp = 1; //solo escribimos 1 byte
                ++bytes_accessed;
                ++mem_access;
            }
            else{
                //acceso de lectura
                replacement = 1; //hay que reemplazar
                tag_out = cache[linea_mc].tag;
                cache[linea_mc].tag = tag;  //cambiamos los tags
                
                //traemos bloque de memoria principal
                lec_mp = 1; //leemos de memoria principal
                mida_lec_mp = 32;   //32 bytes
                bytes_accessed+=32;
                ++mem_access;
            }
        }
    }
    //si la linea no est‡ disponible hay que cargarla por primera vez
    else{
        ++misses;
        replacement = 0;    //no hay linea que reemplazar
        miss = 1;   //hay fallo de acceso
        //si hacemos un acceso de lectura hay que traer el bloque de memoria principal, sino s—lo escribimos
        if(LE){
            //acceso de escritura
            esc_mp = 1; //hay que escribir en memoria principal;
            mida_esc_mp = 1; //solo escribimos 1 byte
            ++bytes_accessed;
            ++mem_access;
        }
        else{
            //acceso de lectura (traemos todo el bloque y dejamos la l’nea como v‡lida)
            cache[linea_mc].valido = 1;
            cache[linea_mc].tag = tag;
            lec_mp = 1;
            mida_lec_mp = 32;
            bytes_accessed+=32;
            ++mem_access;
        }
    }
    
    /* La funcio test_and_print escriu el resultat de la teva simulacio
     * per pantalla (si s'escau) i comproba si hi ha algun error
     * per la referencia actual
     * */
    test_and_print (address, LE, byte, bloque_m, linea_mc, tag,
            miss, lec_mp, mida_lec_mp, esc_mp, mida_esc_mp,
            replacement, tag_out);
}

/* La rutina final es cridada al final de la simulacio */
void final (){
    printf("Numero de fallos: %d\nNumero de aciertos: %d\nNumero de accesos: %d\n",misses,hits,misses+hits);
    printf("Bytes accedidos a memoria: %d\nAccesos a memoria: %d\n",bytes_accessed, mem_access);
}
