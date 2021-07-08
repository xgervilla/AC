#include "CacheSim.h"
#include <stdio.h>

#define not !   //define para que sea más visible (y entendible comparando con c++) la negación

#define num_lineas 64 //(2^2 * 2^10 (4Kb tam. cache) / 2^5 (32bytes x línea)) 2 (2-asocitiva) = 2^6 líneas -> 6 bits para conj_mc
#define offset 5    //2^5 (32bytes x línea) -> 5 bits para byte
#define tag_desp 11      //bits de tag = 32 (bits x word) - 12 (7 de conj_mc+5 de byte) -> para quedarnos con el tag desplazamos los bits inncesarios (12)

//linea de cache: bit de válido y tag (no hace falta implementar la escritura de datos)
typedef struct{
    unsigned int valid;
    int tag;
} linea_cache;

//bloque de memoria: 2 blocs (vias, líneas de cache de 32kb) y comodín para simular el algoritmo LRU
typedef struct{
    linea_cache bloc[2];
    unsigned int ultimo; //"comodin" que indica el bloque que ha sido modificado el último (complementario del bloc en el que estamos
} bloque;

//vector de lineas de cache: cache
bloque cache[num_lineas];

//contadores de misses y hits (informacion para imprimir en pantalla al acabar)
int misses, hits;


void init_cache ()
{
    totaltime=0.0;
    misses = hits = 0;  //iniciamos el contador de misses y hits a 0
    for(int i = 0; i<num_lineas; ++i){
        cache[i].bloc[0].valid = 0;
        cache[i].bloc[1].valid = 0;     //dejamos todas las líneas de la cache como no válidas
    }
}

/* La rutina reference es cridada per cada referencia a simular */
void reference (unsigned int address)
{
    unsigned int byte;
    unsigned int bloque_m;
    unsigned int conj_mc;
    unsigned int via_mc;
    unsigned int tag;
    unsigned int miss;       // boolea que ens indica si es miss
    unsigned int replacement;  // boolea que indica si es reemplaça una linia valida
    unsigned int tag_out;       // TAG de la linia reemplaçada
    float t1,t2;        // Variables per mesurar el temps (NO modificar)
        
    t1=GetTime();
    /*hay que traducir la direccion (address) a los valores que corresponden
     *tag = address<0-20>
     *conj_mc = address<21-26>
     *bloque_m = address<0-26>
     *byte = address<27-31>
     */
    tag = address >> tag_desp; //21 bits (32-11)
    bloque_m = address >> offset; //27 bits
    conj_mc = (address & 0x000007E0) >> offset; //6 bits
    byte = address & 0x0000001F; //5 bits
    
    
    /*si la línea de la caché está ocupada (valid == 1) hay que comprobar si el tag coincide (hay hit) o si hay que sustituir (replacement = 1)
     *si la línea de la caché no tiene el bit válido hay que cargar la nueva línea válida
     */
    //comprobamos si está en el primer bloc
    if (cache[conj_mc].bloc[0].valid && cache[conj_mc].bloc[0].tag == tag){
        //hay acierto
        ++hits;
        miss = 0; //no hay miss en este acceso
        replacement = 0; //no hace falta reemplazar
        via_mc = 0;
        cache[conj_mc].ultimo = 1;
    }
    //comprobamos si está en el segundo bloc
    else if (cache[conj_mc].bloc[1].valid && cache[conj_mc].bloc[1].tag == tag){
        //hay acierto
        ++hits;
        miss = 0; //no hay miss en este acceso
        replacement = 0; //no hace falta reemplazar
        via_mc = 1;
        cache[conj_mc].ultimo = 0;
    }
    //si no está en ninguno de los bloques (hay miss)
    else{
        //hay miss
        ++misses;
        miss = 1; //hay miss en este acceso
        //hay que ver si hay que reemplazar uno de los dos bloques o si hay alguno que esté vacío donde cargar la nueva línea
        if(not cache[conj_mc].bloc[0].valid){
            //el bloque 0 está vacío
            via_mc = 0;
            replacement = 0;
            cache[conj_mc].ultimo = 1;
            cache[conj_mc].bloc[0].tag = tag;
            cache[conj_mc].bloc[0].valid = 1;
        }
        else if(not cache[conj_mc].bloc[1].valid){
            //el bloque 1 está vacío
            via_mc = 1;
            replacement = 0;
            cache[conj_mc].ultimo = 0;
            cache[conj_mc].bloc[1].tag = tag;
            cache[conj_mc].bloc[1].valid = 1;
        }
        else{
            //si los dos bloques están ocupados hay que sustituir
            replacement = 1;
            via_mc = cache[conj_mc].ultimo; //hay que coger la via del algoritmo LRU
            tag_out = cache[conj_mc].bloc[via_mc].tag; //sale el tag de la que sustituimos
            cache[conj_mc].bloc[via_mc].tag = tag; //tag del que añadimos
            
            cache[conj_mc].ultimo = not via_mc;     //cambiamos el valor para actualizar LRU
        }
    }
    
    t2=GetTime();
    totaltime+=t2-t1;
    test_and_print2 (address, byte, bloque_m, conj_mc, via_mc, tag,
            miss, replacement, tag_out);
}

/* La rutina final es cridada al final de la simulacio */
void final ()
{
    printf("Numero de fallos: %d\n", misses);
    printf("Numero de aciertos: %d\n", hits);
    printf("Numero total de accesos: %d (misses+hits)\n", misses+hits);
}
