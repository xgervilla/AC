#include "CacheSim.h"
#include <stdio.h>
/* Posa aqui les teves estructures de dades globals
 * per mantenir la informacio necesaria de la cache
 * */

#define num_lineas 128 //2^2 * 2^10 (4Kb tam. cache) / 2^5 (32bytes x línea) = 2^7 líneas -> 7 bits para linea_mc
#define offset 5    //2^5 (32bytes x línea) -> 5 bits para byte
#define tag_desp 12      //desplazamiento para quedarnos con el tag (5 de offset+7 de línea_mc)
/*
 *tag = address<0-19>
 *linea_mc = address<20-26>
 *bloque_m = address<0-26>
 *byte = address<27-31>
 
como es Copy Back + Allocate:
    -en caso de acierto de escritura se escribe en la línea de cache (dirty)
    -en caso de fallo de escritura se trae el bloque y lo escribe (lee 32 bytes de memoria principal)
    -en caso de fallo de lectura se trae el bloque a caché (32 bytes)
    -en caso de acierto de lectura no se hace nada
    -al reemplazar una lína hay que comprobar si está dirty para escribir en memoria principal (escribe 32 bytes)
*/

int hits, misses;
int bytes_accessed;

typedef struct{
    unsigned int valido;
    unsigned int dirty;
    unsigned int tag;
} linea_cache;


linea_cache cache[num_lineas];
/* La rutina init_cache es cridada pel programa principal per
 * inicialitzar la cache.
 * La cache es inicialitzada al comenÁar cada un dels tests.
 * */
void init_cache (){
    hits = misses = 0;
    bytes_accessed = 0;
    for(int i = 0; i<num_lineas; ++i){
        //preparamos la estructura de la caché (dejamos todas las líneas como no válidas y sin modificar -> vacía)
        cache[i].valido = 0;
        cache[i].dirty = 0;
    }
}

/* La rutina reference es cridada per cada referencia a simular */
void reference (unsigned int address, unsigned int LE){
    unsigned int byte;
    unsigned int bloque_m;
    unsigned int linea_mc;
    unsigned int tag;
    unsigned int miss;      //hay fallo al acceder a la linea de caché
    unsigned int lec_mp;    //leemos de memoria principal
    unsigned int mida_lec_mp;   //tamaño de lo que leemos en memoria principal (si leemos)
    unsigned int esc_mp;    //escribimos en memoria principal
    unsigned int mida_esc_mp;   //tamaño de lo que escribimos en memoria principal
    unsigned int replacement;   //reemplazamos linea de caché
    unsigned int tag_out;   //tag de la linea que reemplazamos
    
    //separamos la direccion en los valores que interesan
    tag = address >> tag_desp; //20 bits (32-12)
    bloque_m = address >> offset; //27 bits
    linea_mc = (address & 0x00000FE0) >> offset; //7 bits
    byte = address & 0x0000001F; //5 bits
    
    //dejamos los valores de acceso a memoria principal en 0, si hay que acceder ya se modificarán
    lec_mp = esc_mp = mida_esc_mp = mida_lec_mp = 0;
    
    //comprobamos si la linea está disponible
    if(cache[linea_mc].valido){
        //comprobamos si coincide el tag (hay hit)
        if(cache[linea_mc].tag == tag){
            //hay hit
            ++hits;
            replacement = 0;
            miss = 0;
            
            //comprobamos si es escritura (si es lectura no hace falta modificar nada más)
            if (LE){
                //hay acceso de escritura -> modificamos el valor dirty
                cache[linea_mc].dirty = 1;
            }
        }
        //si el tag no coincide hay miss
        else{
            ++misses;
            miss = 1;
            replacement = 1;
            tag_out = cache[linea_mc].tag;
            cache[linea_mc].tag = tag;  //cambiamos los tags para reemplazar
            //tanto en escritura como lectura hay que reemplazar, hay que comprobar si dirty está activo
            if(cache[linea_mc].dirty){
                //si está dirty hay que escribir la línea actual en memoria principal
                esc_mp = 1;
                mida_esc_mp = 32;
                //dejamos el valor dirty de la nueva línea a 0
                cache[linea_mc].dirty = 0;
                ++bytes_accessed;
            }
            
            //como hay política Copy Back tanto en lectura como escritura hay que leer el bloque de memoria
            lec_mp = 1; //leemos de memoria principal
            mida_lec_mp = 32;   //32 bytes
            ++bytes_accessed;
            
            if(LE){
                //acceso de escritura
                cache[linea_mc].dirty = 1;
            }
        }
    }
    
    //si la linea no está disponible hay que cargarla por primera vez
    else{
        ++misses;
        replacement = 0;    //no hay linea que reemplazar
        miss = 1;   //hay fallo de acceso
        //tanto si es lectura como escritura traemos el bloque de memoria principal
        lec_mp = 1;
        mida_lec_mp = 32;
        ++bytes_accessed;
        //dejamos la nueva línea como válida
        cache[linea_mc].valido = 1;
        cache[linea_mc].tag = tag;
        //si es un acceso de escritura dejamos el valor dirty a 1, sino lo dejamos a 0
        if(LE){
            cache[linea_mc].dirty = 1;
        }
        else{
            cache[linea_mc].dirty = 0;
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
    printf("Bytes accedidos a memoria: %d\nAccesos a memoria: %d\n", bytes_accessed*32, bytes_accessed);
}

//Tanto los hits, misses y bytes accedidos son similares pero en esta política (Copy Back + Allocate) se accede menos veces. Si asumimos que la diferencia de tiempo entre acceder por 1 byte o por 32 es redundante podemos afirmar que esta política de escritura es mejor para la versión 2.
