#include "CacheSim.h"
#include <stdio.h>


#define num_lineas 128 //2^2 * 2^10 (4Kb tam. cache) / 2^5 (32bytes x l’nea) = 2^7 l’neas -> 7 bits para linea_mc
#define offset 5    //2^5 (32bytes x l’nea) -> 5 bits para byte
#define tag_desp 12      //bits de tag = 32 (bits x word) - 12 (7 de linea_mc+5 de byte) -> para quedarnos con el tag desplazamos los bits inncesarios (12)

//linea de cache: bit de v‡lido y tag (no hace falta implementar la escritura de datos)
typedef struct{
    unsigned int valid;
    int tag;
} linea_cache;

//vector de lineas de cache: cache
linea_cache cache[num_lineas];

//contadores de misses y hits (informacion para imprimir en pantalla al acabar)
int misses, hits;


void init_cache ()
{
    totaltime=0.0;
    misses = hits = 0;  //iniciamos el contador de misses, hits y accesos a 0
    for(int i = 0; i<num_lineas; ++i){
        cache[i].valid = 0;     //dejamos todas las l’neas de la cache como no v‡lidas
    }
}

/* La rutina reference es cridada per cada referencia a simular */
void reference (unsigned int address)
{
    unsigned int byte;
    unsigned int bloque_m;
    unsigned int linea_mc;
    unsigned int tag;
    unsigned int miss;       // boolea que ens indica si es miss
    unsigned int replacement;  // boolea que indica si es reemplaça una linia valida
    unsigned int tag_out;       // TAG de la linia reemplaada
    float t1,t2;        // Variables per mesurar el temps (NO modificar)
    
    t1=GetTime();
    /*hay que traducir la direccion (address) a los valores que corresponden
     *tag = address<0-19>
     *linea_mc = address<20-26>
     *bloque_m = address<0-26>
     *byte = address<27-31>
     */
    tag = address >> tag_desp; //20 bits (32-12)
    bloque_m = address >> offset; //27 bits
    linea_mc = (address & 0x00000FE0) >> offset; //7 bits
    byte = address & 0x0000001F; //5 bits
    
    
    /*si la l’nea de la cachŽ est‡ ocupada (valid == 1) hay que comprobar si el tag coincide (hay hit) o si hay que sustituir (replacement = 1)
     *si la l’nea de la cachŽ no tiene el bit v‡lido hay que cargar la nueva l’nea v‡lida
     */
    if (cache[linea_mc].valid) {
        //comprobamos si el tag coincide
        if (cache[linea_mc].tag == tag) {
            //hay acierto
            ++hits;
            miss = 0; //no hay miss en este acceso
            replacement = 0; //no hace falta reemplazar
        }
        else {
            //el tag no coincide (hay fallo) -> hay que reemplazar
            ++misses;
            miss = 1; //hay miss en este acceso
            replacement = 1; //hay que reemplazar
            tag_out = cache[linea_mc].tag; //el tag que reemplazamos (tag_out) es el que ya hab’a
            cache[linea_mc].tag = tag; //guardamos el nuevo tag
        }
    }
    else{
        //hay que cargar por primera vez en esa l’nea (hay fallo)
        ++misses;
        miss = 1; //hay miss en este acceso
        replacement = 0;
        cache[linea_mc].valid = 1; //marcamos como v‡lido esa l’nea
        cache[linea_mc].tag = tag; //guardamos el nuevo tag
    }
    t2=GetTime();
    totaltime+=t2-t1;
    test_and_print (address, byte, bloque_m, linea_mc, tag,
            miss, replacement, tag_out);
}

/* La rutina final es cridada al final de la simulacio */
void final ()
{
    printf("Numero de fallos: %d\n", misses);
    printf("Numero de aciertos: %d\n", hits);
    printf("Numero total de accesos: %d (misses+hits)\n", misses+hits);
}
