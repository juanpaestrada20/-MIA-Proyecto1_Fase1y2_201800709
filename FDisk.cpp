#include "FDisk.h"

FDisk::FDisk(int size, char unit, QString path, QString type, QString fit, QString eliminar, QString name, int add, int opcion)
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
        if(this->add!=0){
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

}

void FDisk::Particion_Delete(bool raid){

}

void FDisk::Crear_Particion(bool raid){
    MBR mbr;
    FILE *disco = fopen(this->path.toStdString().c_str(), "r+b");
    if(disco==NULL){
        printf("No se puedo encontrar el disco, verifique la ruda");
        return;
    }
    fread(&mbr, sizeof (MBR), 1, disco);

    int peso = sizeof (mbr);
    bool espacioParticion = false;
    int sizePartition = Calcular_Espacio();
    int corrida = peso;
    int NoEspacio = false;

    bool auxLogica = false;
    int posLogica = false;

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
                    if(isPartition){
                        int rest = nextPartition - corrida;
                        if(sizePartition <= rest){
                            if(!Nombre_noRepetido(mbr, disco)){
                                mbr.partitions[i] = LLenar_Particion(corrida);
                                rewind(disco);
                                fwrite(&mbr, sizeof (mbr), 1, disco);
                                printf("Se creo la particion exitosamente!\n");
                                NoEspacio = true;
                                break;
                            }else{
                                NoEspacio = true;
                                printf("Nombre creado para la particion no valida\n");
                            }
                        }else{
                            NoEspacio = true;
                            printf("No se puede crear la particion, el espacion no es suficiente\n");
                        }
                    }else{
                        corrida = mbr.partitions[i].start +mbr.partitions[i].size;
                        if(i==3){
                            espacioParticion = true;
                            NoEspacio = true;
                        }
                    }

                }
            }
        } else if(mbr.disk_fit == 'w' ){
            printf("Es worst fit \n");
        } else if(mbr.disk_fit == 'b' ){
            printf("Es best fit \n");
        }
        break;
    }
    case 'e':
    {
        break;
    }
    case 'l':
    {
        break;
    }
    }

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

}

Partition FDisk::LLenar_Particion(int inicio){

}
