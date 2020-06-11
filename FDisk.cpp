#include "FDisk.h"

FDisk::FDisk(int size, char unit, QString path, QString type, QString fit, QString eliminar, string name, int add, int opcion)
{
    this->size = size;
    this->unit = unit;
    this->path = path;
    this->type = type;
    this->fit = fit;
    this->eliminar = eliminar;
    this->name = name;
    this->add = add;
    this->opcion_exe = opcion;
}

void FDisk::Ejecutar(){
    Validar_Fdisk();

}

void FDisk::Validar_Fdisk(){
    if(this->path != ""){
        if(this->opcion_exe==1){
            //Si se cumple es que se agregara o se quitara una particion
            Particion_Add(true);
        }else if(this->eliminar != ""){
            //Si se cumple es para eliminar una particion
            if(this->name != ""){
                Particion_Delete(true);
            }else{
                printf("Falta parametro name\n");
            }
        }else{
            // Para crear una particion
            if(this->size > 0){
                Crear_Particion(true);
            }else{
                printf("Hizo falta parametro size\n");
            }
        }
    }else{
        printf("Hace falta parametro path\n");
    }
}

void FDisk::Particion_Add(bool raid){
    MBR mbr;

    FILE* disco;
    disco = fopen(this->path.toStdString().c_str(),"r+b");

    if(disco == NULL){
        if(raid)
            printf("No se puede abrir el disco, Verifique la ruta o si existe el disco\n");
        return;
    }
    fread(&mbr,sizeof(MBR),1,disco);

    int espacio = Calcular_Espacio_Add();
    int posicion = 0;
    bool encontrado = false;
    Partition part;

    for(int i=0; i<4 ;i++)
    {
        if( strcmp(mbr.partitions[i].name, this->name.c_str()) == 0 && mbr.partitions[i].status != '0')
        {
            encontrado = true;
            posicion = i;

            part = mbr.partitions[i];
            break;
        }
    }

    if(encontrado)//ENCONTRO LA PARTICION CON EL MISMO NOMBRE
    {
        if(opcion_exe == 0)//SI ES TRUE VAMOS A QUITAR ESPACIO
        {
            if(espacio >= part.size)//SI EL ESPACIO A QUITAR ES MAYOR AL DISPONIBLE MENSAJE ERROR
            {
                if(raid)
                    printf("El espacio deseado a quitar es mas de lo hay disponible en la particion en este disco\n");
                return;
            }else//SI NO PUES SE PUEDE ELIMINAR EL ESPACIO
            {
                part.size = part.size - espacio;

                mbr.partitions[posicion] = part;

                rewind(disco);//REINICIO EL APUNTADOR AL INCIO DEL DISCO
                fwrite(&mbr,sizeof(mbr),1, disco);//ACTUALIZO EL MBR
            }



        }else//SI ES FALSE VAMOS AGREGAR ESPACIO
        {
            if(posicion == 3)//SI ES TRUE ES PORQUE ES EL ULTIMO PEDAZO
            {
                int sobrante = mbr.size - (part.start + part.size);
                if(sobrante >= espacio)
                {
                    part.size = part.size + espacio;

                    mbr.partitions[posicion] = part;

                    rewind(disco);//REINICIO EL APUNTADOR AL INCIO DEL DISCO
                    fwrite(&mbr,sizeof(mbr),1, disco);//ACTUALIZO EL MBR
                }else
                {
                    if(raid)
                        printf("El espacio deseado a agregar es mas de lo hay disponible en la particion en este disco\n");

                    return;
                }

            }else
            {
                //Particion siguiente = mbr.partitions[posicion + 1];
                //int sobrante = siguiente.part_start - (part.part_start + part.part_size);

                Partition siguiente;
                int sobrante = 0;
                int posSiguientes = posicion+1;

                while(posSiguientes < 5 )
                {
                    if(posSiguientes == 4)//ESTE NOS INDICA QUE NO HAY, ENTONCES HAY QUE HACERLO CON EL TAMANIO DEL DISCO
                    {
                        sobrante = mbr.size - (part.start + part.size);
                        break;
                    }
                    else
                    {
                        siguiente = mbr.partitions[posSiguientes++];

                        if(siguiente.status != '0')//VERIFICAMOS SI HAY PARTICION
                        {
                            sobrante = siguiente.start - (part.start + part.size);
                            break;
                        }
                    }

                }


                if(sobrante >= espacio)
                {
                    part.size = part.size + espacio;

                    mbr.partitions[posicion] = part;

                    rewind(disco);//REINICIO EL APUNTADOR AL INCIO DEL DISCO
                    fwrite(&mbr,sizeof(mbr),1, disco);//ACTUALIZO EL MBR
                }else
                {
                    if(raid)
                        printf("El espacio deseado a agregar es mas de lo hay disponible en la particion en este disco\n");

                    return;
                }
            }


        }
        if(raid)
            printf("El espacio de la particion en este disco a sido modificado\n");

    }else
    {
        if(raid)
            printf("No se encontro ninguna particion con el nombre en este disco\n");
    }

    fclose(disco);

    if(raid)//SI ES TRUE MODIFICO SU RAID
    {
        Ruta_Raid();
        Particion_Add(false);
    }
}

void FDisk::Particion_Delete(bool raid){
    MBR mbr;
    FILE *disco = fopen(this->path.toStdString().c_str(), "r+b");
    if(disco == NULL){
        if(raid){
            printf("No se ha podido abrir el disco\n");
        }
        return;
    }
    fread(&mbr, sizeof (MBR), 1, disco);
    bool encontrado = false;
    int posicion = 0;
    for(int i = 0; i < 4; i++){
        if(strcmp(mbr.partitions[i].name, this->name.c_str()) == 0 && mbr.partitions[i].status != 0){
            encontrado = true;
            posicion = i;
        }
    }
    bool bandera = true;
    do{


        if(this->eliminar.toStdString()[1] == 'a')//========== ELEMINACION FAST
        {
            if(encontrado)//ENCONTRO LA PARTICION CON EL MISMO NOMBRE
            {
                if(mbr.partitions[posicion].type == 'p'){
                    mbr.partitions[posicion].status = '0';
                    rewind(disco);
                    fwrite(&mbr,sizeof(MBR),1,disco);
                    if(raid){
                        printf("Se borro la particion con el nombre en este disco\n");
                    }
                    bandera = false;
                }else if(mbr.partitions[posicion].type == 'e')
                {
                    mbr.partitions[posicion].status = '0';

                    rewind(disco);
                    fwrite(&mbr,sizeof(MBR),1,disco);

                    if(raid){
                        printf("Se borro la particion con el nombre en este disco\n");
                    }
                    bandera = false;
                }
            }else
            {
                if(raid){
                    printf("No se encontro ninguna particion con el nombre en este disco\n");
                }
                bandera = false;
            }

        }else if(this->eliminar.toStdString()[1] == 'u')//ELIMINACION FULL
        {

            if(encontrado)//ENCONTRO LA PARTICION CON EL MISMO NOMBRE
            {
                if(mbr.partitions[posicion].type == 'p')
                {

                    fseek(disco, mbr.partitions[posicion].start, SEEK_SET);

                    char vaciar[mbr.partitions[posicion].size];

                    fwrite(vaciar,sizeof(vaciar),mbr.partitions[posicion].size,disco);

                    rewind(disco);

                    mbr.partitions[posicion] = Vaciar_Particion();

                    fwrite(&mbr,sizeof(MBR),1,disco);
                    if(raid){
                        printf("Se borro la particion con el nombre en este disco ***\n");
                    }

                }else if(mbr.partitions[posicion].type == 'e')
                {

                    fseek(disco, mbr.partitions[posicion].start, SEEK_SET);

                    //fwrite(&mbr.partitions[posicion],sizeof(Particion),1,disco);

                    char vaciar[mbr.partitions[posicion].size];

                    fwrite(vaciar,sizeof(vaciar),mbr.partitions[posicion].size,disco);

                    rewind(disco);

                    mbr.partitions[posicion] = Vaciar_Particion();

                    fwrite(&mbr,sizeof(MBR),1,disco);
                    if(raid){
                        printf("Se borro la particion con el nombre en este disco\n");
                    }

                }
                bandera = false;
            }else{

                if(raid){
                    printf("No se encontro ninguna particion con el nombre en este disco\n");
                }
                bandera = false;
            }
        }



    }
    while(bandera);


    fclose(disco);

    if(raid)//SI ES TRUE LO VUELVO A EJECUTAR
    {

        Ruta_Raid();
        Particion_Delete(false);
    }
}

void FDisk::Crear_Particion(bool raid){
    MBR mbr;
    FILE *disco = fopen(this->path.toStdString().c_str(), "r+b");
    if(disco==NULL){
        if(raid){
            printf("No se puedo encontrar el disco, verifique la ruta\n");
        }
        return;
    }
    fread(&mbr, sizeof (MBR), 1, disco);

    int peso = sizeof (mbr);
    bool espacioParticion = true;
    int sizePartition = Calcular_Espacio();
    int corrida = peso;
    int NoEspacio = false;

    bool auxLogica = false;
    int posLogica = false;

    // verifico si tiene espacio para una particion
    for(int i = 0; i < 4; i++){
        if(mbr.partitions[i].status == '0'){
            espacioParticion = false;
            break;
        }
    }

    if(espacioParticion && this->type != "l"){
        if(raid)
            printf("En el disco seleccionado no se pueden hacer mas particiones!\n");
        fclose(disco);
        return;
    }

    //Verificar el tipo de particion
    switch (this->type.toStdString()[0]) {
    //Particion Primaria
    case 'p':
    {
        //Verificar el tipo de fit
        if(mbr.disk_fit == 'f' ){
            //Buscamos espacio vacio
            for(int i = 0; i < 4; i++){
                if(mbr.partitions[i].status == '0'){ //primer espacio vacio encontrado
                    bool isPartition = false;
                    int nextPartition = 0;
                    for(int j = i+1; j < 4; j++){
                        if(mbr.partitions[j].status != '0'){ //particion contigua a el
                            isPartition = true;
                            nextPartition = mbr.partitions[j].start;
                            break;
                        }
                    }
                    if(isPartition){ //Hay particion adelante
                        int rest = nextPartition - corrida;
                        if(sizePartition <= rest && sizePartition > rest){//Verificar si hay espacio
                            if(!Nombre_noRepetido(mbr, disco)){
                                mbr.partitions[i] = LLenar_Particion(corrida);
                                rewind(disco);
                                fwrite(&mbr, sizeof (mbr), 1, disco);
                                if(raid)
                                    printf("Se ha creado la particion %s satisfactoriamente\n", mbr.partitions[i].name);
                                NoEspacio = true;
                                break;
                            }else{
                                NoEspacio = true;
                                if(raid)
                                    printf("No se pueden repetir los nombres de la particion\n");
                                break;
                            }
                        }else{
                            if(i==3){
                                NoEspacio = true;
                                if(raid)
                                    printf("No se puede crear la particion, el espacio no es suficiente\n");
                                break;
                            }
                        }
                    }else{ //No hay particion delante
                        int resto = mbr.size - corrida;
                        if(sizePartition <= resto && sizePartition > 0){//Verificar el espacio
                            if(!Nombre_noRepetido(mbr, disco)){
                                mbr.partitions[i] = LLenar_Particion(corrida);
                                rewind(disco);
                                fwrite(&mbr, sizeof (mbr), 1, disco);
                                if(raid)
                                    printf("Se creo la particion exitosamente!\n");

                                NoEspacio = true;
                                break;
                            }else{
                                NoEspacio = true;
                                if(raid)
                                    printf("No se pueden repetir los nombres de la particion\n");
                                break;
                            }
                        }else{
                            NoEspacio = true;
                            if(raid)
                                printf("No se puede crear la particion, el espacio no es suficiente\n");
                            break;
                        }
                    }

                }else{
                    corrida = mbr.partitions[i].start + mbr.partitions[i].size;
                    if(i==3){
                        espacioParticion = true;
                        NoEspacio = true;
                    }
                }
            }
        } else if(mbr.disk_fit == 'w' ){
            int nextParticion=0;        //INICIO DE LA SIGUIENTE PARTICION
            int tamanioMayor = 0;
            int posParticion = 0;
            int resto = 0;

            bool bandera = false; //ME INDICA SI NO HAY PARTICIONES

            for(int i = 0; i < 4; i ++){// buscar particion disponible
                if(mbr.partitions[i].status == '0'){// particion disponible
                    for(int j = i+1; j < 4;  j++){
                        if(mbr.partitions[j].status != '0'){ // particion mas adelante
                            bandera = true;
                            nextParticion = mbr.partitions[j].start;
                            resto = nextParticion - corrida;
                            if(resto > tamanioMayor){//ver si el nuevo espacio es mas grande
                                tamanioMayor = nextParticion - corrida;
                                posParticion = i;

                            }
                        }
                    }
                    if(!bandera){
                        posParticion = i;
                        resto = mbr.size - corrida;
                        break;
                    }
                    if(i == 3){
                        resto = mbr.size - corrida;
                        if( resto >  tamanioMayor){
                            tamanioMayor =mbr.size - corrida;
                            posParticion = i;
                        }
                    }
                }else{
                    corrida = mbr.partitions[i].start + mbr.partitions[i].size;
                    if(i ==3 && tamanioMayor != 0){
                        espacioParticion = true;
                        NoEspacio = true;
                    }
                }
            }
            if(sizePartition <= mbr.size && sizePartition > 0){
                if(!Nombre_noRepetido(mbr, disco)){
                    mbr.partitions[posParticion] = LLenar_Particion(corrida);
                    rewind(disco);
                    fwrite(&mbr, sizeof (mbr), 1, disco);
                    if(raid)
                        printf("Se ha creado la particion %s satisfactoriamente\n", mbr.partitions[posParticion].name);

                    NoEspacio = true;
                    break;
                }else{
                    NoEspacio = true;
                    if(raid)
                        printf("No se pueden repetir los nombres de la particion\n");
                    break;
                }
            }else{
                NoEspacio = true;
                if(raid)
                    printf("No se puede crear la particion, el espacio no es suficiente\n");
                break;
            }

        } else if(mbr.disk_fit == 'b' ){

            int nextParticion=0;        //INICIO DE LA SIGUIENTE PARTICION
            int tamanioMayor = 0;
            int posParticion = 0;
            int resto = 0;

            bool bandera = false;

            for(int i = 0; i < 4; i++){
                if(mbr.partitions[i].status == '0'){
                    for(int j = i+1; j < 4; j++){
                        if(mbr.partitions[j].status != '0'){
                            bandera = true;
                            nextParticion = mbr.partitions[j].start;
                            resto = nextParticion - corrida;
                            if(resto< tamanioMayor && sizePartition <= resto && sizePartition > 0){
                                tamanioMayor = nextParticion - corrida;
                                posParticion = i;
                            }else if( tamanioMayor == 0){
                                tamanioMayor = nextParticion - corrida;
                                posParticion = i;
                            }
                        }
                    }
                    if(!bandera){
                        posParticion = i;
                        resto = mbr.size - corrida;
                    }
                    if(i == 3){
                        resto = mbr.size - corrida;
                        if(resto < tamanioMayor && sizePartition <= size && sizePartition > 0){
                            tamanioMayor = mbr.size-corrida;
                            posParticion = i;
                        }else if(tamanioMayor==0){
                            tamanioMayor = mbr.size - corrida;
                            posParticion = i;
                        }
                    }
                }else{
                    corrida = mbr.partitions[i].start + mbr.partitions[i].size;
                    resto = mbr.size -corrida;
                    if(i == 3 && tamanioMayor != 0){
                        espacioParticion = true;
                        NoEspacio = true;
                    }
                }
            }
            if(sizePartition <= resto && sizePartition > 0){
                if(!Nombre_noRepetido(mbr, disco)){
                    mbr.partitions[posParticion] = LLenar_Particion(corrida);
                    rewind(disco);
                    fwrite(&mbr,sizeof (mbr),1,disco);
                    if(raid)
                        printf("Se ha creado la particion %s satisfactoriamente\n", mbr.partitions[posParticion].name);

                    NoEspacio = true;
                    break;
                }else{
                    NoEspacio = true;
                    if(raid)
                        printf("No se pueden repetir los nombres de la particion\n");
                    break;
                }
            }else{
                NoEspacio = true;
                if(raid)
                    printf("No se puede crear la particion, el espacio no es suficiente\n");
                break;
            }
        }
        if(!NoEspacio){
            if(raid)
                printf("No se puede crear la particion\n");
        }
        break;
    }
    case 'e':
    {
        if(mbr.disk_fit == 'f' ){
            //Buscamos espacio vacio
            for(int i = 0; i < 4; i++){
                if(mbr.partitions[i].status == '0'){ //primer espacio vacio encontrado
                    bool isPartition = false;
                    int nextPartition = 0;
                    for(int j = i+1; j < 4; j++){
                        if(mbr.partitions[j].status != '0'){ //particion contigua a el
                            isPartition = true;
                            nextPartition = mbr.partitions[j].start;
                            break;
                        }
                    }
                    if(isPartition){ //Hay particion adelante
                        int rest = nextPartition - corrida;
                        if(sizePartition <= rest && sizePartition > rest){//Verificar si hay espacio
                            if(!Nombre_noRepetido(mbr, disco)){
                                if(!Existe_Extendida(mbr)){
                                    mbr.partitions[i] = LLenar_Particion(corrida);
                                    rewind(disco);
                                    fwrite(&mbr, sizeof (mbr), 1, disco);
                                    fseek(disco, mbr.partitions[i].start, SEEK_SET);
                                    EBR ebr = LLenar_EBR(corrida);
                                    fwrite(&ebr,sizeof (ebr), 1, disco);
                                    if(raid)
                                        printf("Se ha creado la particion extendida %s satisfactoriamente en \n", mbr.partitions[i].name);

                                    NoEspacio = true;
                                    break;
                                }else{
                                    NoEspacio = true;
                                    if(raid)
                                        printf("No se puede crear la particion extendida\n");
                                    break;
                                }
                            }else{
                                NoEspacio = true;
                                if(raid)
                                    printf("No se pueden repetir los nombres de la particion\n");
                                break;
                            }
                        }else{
                            if(i==3){
                                NoEspacio = true;
                                if(raid)
                                    printf("No se puede crear la particion extendida, el espacio no es suficiente\n");
                                break;
                            }
                        }
                    }else{ //No hay particion delante
                        int resto = mbr.size - corrida;
                        if(sizePartition <= resto && sizePartition > 0){//Verificar el espacio
                            if(!Nombre_noRepetido(mbr, disco)){
                                if(!Existe_Extendida(mbr)){
                                    mbr.partitions[i] = LLenar_Particion(corrida);
                                    rewind(disco);
                                    fwrite(&mbr, sizeof (mbr), 1, disco);
                                    fseek(disco, mbr.partitions[i].start, SEEK_SET);
                                    EBR ebr;
                                    fwrite(&ebr,sizeof (ebr),1,disco);
                                    if(raid)
                                        printf("Se ha creado la particion extendida %s satisfactoriamente en \n", mbr.partitions[i].name);

                                    NoEspacio = true;
                                }else{
                                    NoEspacio = true;
                                    if(raid)
                                        printf("No se puede crear la particion extendida\n");

                                }
                                break;
                            }else{
                                NoEspacio = true;
                                if(raid)
                                    printf("No se pueden repetir los nombres de la particion\n");
                                break;
                            }
                        }else{
                            NoEspacio = true;
                            if(raid)
                                printf("No se puede crear la particion, el espacio no es suficiente\n");
                            break;
                        }
                    }

                }else{
                    corrida = mbr.partitions[i].start + mbr.partitions[i].size;
                    if(i==3){
                        espacioParticion = true;
                        NoEspacio = true;
                    }
                }
            }
        } else if(mbr.disk_fit == 'w' ){
            int nextParticion=0;        //INICIO DE LA SIGUIENTE PARTICION
            int tamanioMayor = 0;
            int posParticion = 0;
            int resto = 0;

            bool bandera = false; //ME INDICA SI NO HAY PARTICIONES

            for(int i = 0; i < 4; i ++){// buscar particion disponible
                if(mbr.partitions[i].status == '0'){// particion disponible
                    for(int j = i+1; j < 4;  j++){
                        if(mbr.partitions[j].status != '0'){ // particion mas adelante
                            bandera = true;
                            nextParticion = mbr.partitions[j].start;
                            resto = nextParticion - corrida;
                            if(resto > tamanioMayor){//ver si el nuevo espacio es mas grande
                                tamanioMayor = nextParticion - corrida;
                                posParticion = i;
                            }
                        }
                    }
                    if(!bandera){
                        posParticion = i;
                        resto = mbr.size - corrida;
                        break;
                    }
                    if(i == 3){
                        resto = mbr.size - corrida;
                        if( resto >  tamanioMayor){
                            tamanioMayor =mbr.size - corrida;
                            posParticion = i;
                        }
                    }
                }else{
                    corrida = mbr.partitions[i].start + mbr.partitions[i].size;
                    if(i ==3 && tamanioMayor != 0){
                        espacioParticion = true;
                        NoEspacio = true;
                    }
                }
            }
            if(sizePartition <= mbr.size && sizePartition > 0){
                if(!Nombre_noRepetido(mbr, disco)){
                    if(!Existe_Extendida(mbr)){
                        EBR ebr = LLenar_EBR(corrida);
                        mbr.partitions[posParticion] = LLenar_Particion(corrida);
                        rewind(disco);
                        fwrite(&mbr, sizeof (mbr), 1, disco);
                        fseek(disco,mbr.partitions[posParticion].start,SEEK_SET);
                        fwrite(&ebr, sizeof(ebr), 1, disco);

                        if(raid)
                            printf("Se ha creado la particion extendida %s satisfactoriamente en \n", mbr.partitions[posParticion].name);
                        NoEspacio = true;

                    }else{
                        NoEspacio = true;
                        if(raid)
                            printf("No se puede crear la particion extendida\n");
                        break;
                    }
                }else{
                    NoEspacio = true;
                    if(raid)
                        printf("No se pueden repetir los nombres de la particion\n");
                    break;
                }
            }else{
                NoEspacio = true;
                if(raid)
                    printf("No se puede crear la particion, el espacio no es suficiente\n");
                break;
            }

        } else if(mbr.disk_fit == 'b' ){

            int nextParticion=0;        //INICIO DE LA SIGUIENTE PARTICION
            int tamanioMayor = 0;
            int posParticion = 0;
            int resto = 0;

            bool bandera = false;

            for(int i = 0; i < 4; i++){
                if(mbr.partitions[i].status == '0'){
                    for(int j = i+1; j < 4; j++){
                        if(mbr.partitions[j].status != '0'){
                            bandera = true;
                            nextParticion = mbr.partitions[j].start;
                            resto = nextParticion - corrida;
                            if(resto < tamanioMayor && sizePartition <= resto && sizePartition > 0){
                                tamanioMayor = nextParticion - corrida;
                                posParticion = i;
                            }else if( tamanioMayor == 0){
                                tamanioMayor = nextParticion - corrida;
                                posParticion = i;
                            }
                        }
                    }
                    if(!bandera){
                        posParticion = i;
                        resto = mbr.size - corrida;
                        break;
                    }
                    if(i == 3){
                        resto = mbr.size - corrida;
                        if(resto < tamanioMayor && sizePartition <= size && sizePartition > 0){
                            tamanioMayor = mbr.size-corrida;
                            posParticion = i;
                        }else if(tamanioMayor==0){
                            tamanioMayor = mbr.size - corrida;
                            posParticion = i;
                        }
                    }
                }else{
                    corrida = mbr.partitions[i].start + mbr.partitions[i].size;
                    resto = mbr.size -corrida;
                    if(i == 3 && tamanioMayor != 0){
                        espacioParticion = true;
                        NoEspacio = true;
                    }
                }
            }
            if(sizePartition <= resto && sizePartition > 0){
                if(!Nombre_noRepetido(mbr, disco)){
                    if(!Existe_Extendida(mbr)){
                        EBR ebr =LLenar_EBR(corrida);
                        mbr.partitions[posParticion] = LLenar_Particion(corrida);
                        rewind(disco);
                        fwrite(&mbr, sizeof (mbr), 1, disco);
                        fseek(disco,mbr.partitions[posParticion].start,SEEK_SET);
                        //CERO PARA DECIR QUE NO HAY
                        fwrite(&ebr, sizeof(ebr), 1, disco);
                        if(raid)
                            printf("Se ha creado la particion extendida %s satisfactoriamente en \n", mbr.partitions[posParticion].name);

                        NoEspacio = true;
                    }else{
                        NoEspacio = true;
                        if(raid)
                            printf("El disco ya cuenta con una particion extendida\n");
                    }
                    break;
                }else{
                    NoEspacio = true;
                    if(raid)
                        printf("No se pueden repetir los nombres de la particion\n");
                    break;
                }
            }else{
                NoEspacio = true;
                if(raid)
                    printf("No se puede crear la particion, el espacio no es suficiente\n");
                break;
            }
        }
        if(!NoEspacio){
            if(raid)
                printf("No se puede crear la particion, el espacio no es suficiente\n");
        }
        break;
    }
    case 'l':
    {
        //Verificar si hay ebr
        for(int i=0; i<4 ;i++)//COMPRUEBO SI HAY EXTENDIDA
        {
            if(mbr.partitions[i].type == 'e' && mbr.partitions[i].status != '0' ){
                auxLogica = true;
                posLogica = i;
                break;
            }
        }

        if(auxLogica){
            if(!Nombre_noRepetido(mbr, disco)){
                fseek(disco, mbr.partitions[posLogica].start, SEEK_SET);
                EBR ebr;
                fread(&ebr, sizeof(EBR),1,disco);
                Partition ext = mbr.partitions[posLogica];
                int pesoEBR = sizeof(EBR);
                corrida = pesoEBR +ebr.start;
                if(true){
                    EBR auxEBR = ebr;
                    int finP = ext.size + ext.start;
                    while(corrida < finP){
                        if(auxEBR.next == 0){
                            int resto = finP - corrida;
                            if(sizePartition<=resto && sizePartition > 0){
                                auxEBR.fit = this->fit.toLower().toStdString()[0];
                                auxEBR.size = sizePartition;
                                auxEBR.next = corrida + sizePartition +1;
                                for(int i = 0; i < 16; i++){
                                    auxEBR.name[i] = this->name[i];
                                }
                                fseek(disco, auxEBR.start, SEEK_SET);
                                fwrite(&auxEBR, sizeof(EBR), 1, disco );//AQUI ACTUALIZO EL EBR YA ESTADO


                                EBR nuevo = LLenar_EBR( corrida + sizePartition );


                                fseek(disco, nuevo.start, SEEK_SET);
                                fwrite(&nuevo, sizeof(EBR), 1, disco );//AQUI ACTUALIZO EL EBR YA ESTADO
                                if(raid)
                                    printf("Se creo la particion logica %s\n", auxEBR.name);
                            }else{
                                if(raid)
                                    printf("No se pudo crear la particion logica por falta de espacio!\n");
                            }
                            break;
                        }else{
                            int as = auxEBR.next;
                            corrida = pesoEBR + as;
                            fseek(disco,as,SEEK_SET);
                            fread(&auxEBR, sizeof (EBR),1,disco);
                        }
                    }
                }
            }else{
                if(raid)
                    printf("No se pueden crear particiones con el mismo nombre\n");
            }

        }else{
            if(raid)
                printf("Se necesita crear una particion extendida anted de crear una particion logica\n");
        }
        break;
    }
    }
    fclose(disco);


    //Ruta_Raid();

}

int FDisk::Calcular_Espacio(){
    int resultado = 0;
    if(this->unit=='b'){
        resultado = this->size;
    } else if(this->unit=='k'){
        resultado = this->size*1024;
    } else if(this->unit=='m'){
        resultado = this->size*1024*1024;
    }
    return resultado;
}

bool FDisk::Nombre_noRepetido(MBR mbr, FILE *disco){
    for(int i = 0; i < 4; i++){
        Partition part = mbr.partitions[i];
        // En el caso que la particion se expandida
        if(part.type == 'e'){
            if(strcmp(part.name, this->name.c_str()) == 0 && part.status != '0'){
                return true;
            }
            fseek(disco, part.size, SEEK_SET); // Posicion donde comienzan las logicas
            int finPart = part.size + part.start;
            EBR ebr;
            int corrida = 0;
            int pesoEBR = sizeof(EBR);
            fread(&ebr, sizeof (EBR), 1, disco);//obtener el ebr inicial
            corrida = pesoEBR + ebr.start;
            while(corrida < finPart){
                if(strcmp(ebr.name, this->name.c_str()) == 0 && ebr.status == '0'){
                    return true;
                }else{
                    if(ebr.next != 0){// si no hay siguiente es porque es el final de la lista
                        int espacio = ebr.next;
                        corrida = pesoEBR +espacio;
                        fseek(disco, espacio, SEEK_SET);
                        fread(&ebr, sizeof(EBR), 1, disco);
                    }else{
                        break;
                    }
                }
            }
        }else{
            // si es primaria
            if(strcmp(part.name, this->name.c_str()) == 0 && part.status != '0'){
                return true;
            }
        }
    }
    return false;
}

Partition FDisk::LLenar_Particion(int inicio){
    Partition p;
    p.status='1';
    p.type=this->type.toLower().toStdString()[0];
    p.fit=this->fit.toLower().toStdString()[0];
    p.start=inicio+1;
    p.size=Calcular_Espacio();
    memset(p.name, '\0',sizeof (p.name));
    for(int i = 0; i < (int) this->name.size(); i++){
        p.name[i] = this->name[i];
    }

    return p;
}

EBR FDisk::LLenar_EBR(int inicio){
    EBR r;
    r.status = '1';
    r.fit = fit.toStdString()[0];
    r.start = inicio+1;
    r.size = 0;
    r.next = 0; //CERO PARA DECIR QUE NO HAY
    for(int i = 0; i < (int) this->name.size(); i++){
        r.name[i] = this->name[i];
    }
    return r;
}

bool FDisk::Existe_Extendida(MBR mbr){
    for(int i=0; i<4 ;i++)
    {
        if(mbr.partitions[i].type == 'e' && mbr.partitions[i].status != '0' )
            return true;
    }
    return false;
}

void FDisk::Estado_Fdisk(){
    printf("Creado Particion\n");
    printf("Tamaño: %d\n", this->size);
    printf("Fit: %cf\n",this->fit.toStdString()[0]);
    printf("Unidad: %c\n", this->unit);
    printf("Path: %s\n", this->path.toStdString().c_str());
}

void FDisk::Ruta_Raid()
{
    string pathRaid = this->path.toStdString().substr(0, path.size()-5);
    pathRaid += "_RAID.disk";
    this->path = QString(pathRaid.c_str());

}


int FDisk::Calcular_Espacio_Add(){
    int r;
        if(this->unit == 'b'){
            r = this->add;
        }else if(this->unit == 'k'){
            r = this->add * 1024;
        }else{
            r = this->add * 1024 * 1024;
        }
        return r;
}


Partition FDisk::Vaciar_Particion(){
    Partition r;

    r.status = '0';
    r.type = 'p';
    r.fit = 'w';
    r.start = 0;
    r.size = 0;
    for(int i = 0; i < 16; i++){
        r.name[i] = '\0';
    }
    return r;
}
