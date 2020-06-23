#include "MV.h"

MV::MV(string path, string dest)
{
    this->path = path;
    this->dest = dest;
}

void MV::Ejecutar(){
    if(login){
        FILE *fp = fopen(daLoguer.direccion.c_str(),"r+b");
        char auxPath[500];
        char auxDest[500];
        strcpy(auxPath,this->path.c_str());
        strcpy(auxDest,this->dest.c_str());
        int carpeta = buscarCarpetaArchivo(fp,auxPath);//Carpeta/archivo a mover
        int destino = buscarCarpetaArchivo(fp,auxDest);
        if(carpeta != -1){
            if(destino != -1){
                bool permisos = permisosLecturaRecursivo(fp,carpeta);
                if(permisos){
                    SuperBloque super;
                    InodoTable inodo;
                    fseek(fp,daLoguer.inicioSuper,SEEK_SET);
                    fread(&super,sizeof(SuperBloque),1,fp);
                    fseek(fp,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*destino,SEEK_SET);
                    fread(&inodo,sizeof(InodoTable),1,fp);
                    bool permisos2 = permisosDeEscritura(inodo.i_perm,(inodo.i_uid == daLoguer.id_user),(inodo.i_gid == daLoguer.id_grp));
                    if(permisos2){
                        char auxP[500];
                        strcpy(auxP,this->path.c_str());
                        moverCarpetaArchivo(fp,carpeta,auxP,destino);
                    }else
                        cout << "El usuario actual no tiene permisos de escritura en la carpeta destino" << endl;
                }else
                    cout << "El usuario actual no tiene permisos en alguna de las carpetas hijas" << endl;
            }else
                cout << "No existe la ruta a donde se movera la carpeta/archivo" << endl;
        }else
            cout << "La carpeta/archivo a mover no existe" << endl;
        fclose(fp);
    }else{
        cout << "Debe iniciar sesion para usar el comando" << endl;
    }
}

void MV::guardarJournal(char *operacion, int tipo, int permisos, char *nombre){
    SuperBloque super;
    Journal registro;
    memset(registro.journal_name,'\0',sizeof(registro.journal_name));
    memset(registro.journal_operation_type,'\0',sizeof(registro.journal_operation_type));
    strcpy(registro.journal_operation_type,operacion);
    strcpy(registro.journal_name,nombre);
    strcpy(registro.operation,nombre);
    registro.journal_type = tipo;
    registro.journal_date = time(0);
    registro.journal_owner = daLoguer.id_user;
    registro.journal_permissions = permisos;
    FILE *fp = fopen(daLoguer.direccion.c_str(),"r+b");
    //Buscar el ultimo journal
    Journal registroAux;
    bool ultimo = false;
    fseek(fp,daLoguer.inicioSuper,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,fp);
    int inicio_journal = daLoguer.inicioSuper + static_cast<int>(sizeof(SuperBloque));
    int final_journal = super.s_bm_inode_start;
    fseek(fp,inicio_journal,SEEK_SET);
    while((ftell(fp) < final_journal) && !ultimo){
        fread(&registroAux,sizeof(Journal),1,fp);
        if(registroAux.journal_type != 1 && registroAux.journal_type != 2)
            ultimo = true;
    }
    fseek(fp,ftell(fp)- sizeof(Journal),SEEK_SET);
    fwrite(&registro,sizeof(Journal),1,fp);
    fclose(fp);
}

int MV::byteInodoBloque(FILE *stream,int pos, char tipo){
    SuperBloque super;
    fseek(stream,daLoguer.inicioSuper,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,stream);
    if(tipo == '1'){
        return (super.s_inode_start + static_cast<int>(sizeof(InodoTable))*pos);
    }else if(tipo == '2')
        return (super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*pos);
    return 0;
}

int MV::buscarCarpetaArchivo(FILE *stream, char* path){
    SuperBloque super;
    InodoTable inodo;
    BloqueCarpeta carpeta;
    BloqueApuntadores apuntador;

    QList<string> lista = QList<string>();
    char *token = strtok(path,"/");
    int cont = 0;
    int numInodo = 0;

    while(token != nullptr){
        lista.append(token);
        cont++;
        token = strtok(nullptr,"/");
    }

    fseek(stream,daLoguer.inicioSuper,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,stream);
    numInodo = super.s_inode_start; //Byte donde inicia el inodo

    for (int cont2 = 0; cont2 < cont; cont2++) {
        fseek(stream,numInodo,SEEK_SET);
        fread(&inodo,sizeof(InodoTable),1,stream);
        int siguiente = 0;
        for(int i = 0; i < 15; i++){
            if(inodo.i_block[i] != -1){//Apuntadores directos
                int byteBloque = byteInodoBloque(stream,inodo.i_block[i],'2');
                fseek(stream,byteBloque,SEEK_SET);
                if(i < 12){
                    fread(&carpeta,sizeof(BloqueCarpeta),1,stream);
                    for (int j = 0; j < 4; j++) {
                        if((cont2 == cont - 1) && (strcasecmp(carpeta.b_content[j].b_name,lista.at(cont2).c_str()) == 0)){//Tendria que ser la carpeta
                            return carpeta.b_content[j].b_inodo;
                        }else if((cont2 != cont - 1) && (strcasecmp(carpeta.b_content[j].b_name,lista.at(cont2).c_str()) == 0)){
                            numInodo = byteInodoBloque(stream,carpeta.b_content[j].b_inodo,'1');
                            siguiente = 1;
                            break;
                        }
                    }
                }else if(i == 12){//Apuntador indirecto
                    fread(&apuntador,sizeof(BloqueApuntadores),1,stream);
                    for(int j = 0; j < 16; j++){
                        if(apuntador.b_pointer[j] != -1){
                            byteBloque = byteInodoBloque(stream,apuntador.b_pointer[j],'2');
                            fseek(stream,byteBloque,SEEK_SET);
                            fread(&carpeta,sizeof(BloqueCarpeta),1,stream);
                            for (int k = 0; k < 4; k++) {
                                if((cont2 == cont - 1) && (strcasecmp(carpeta.b_content[k].b_name,lista.at(cont2).c_str()) == 0)){//Tendria que ser la carpeta
                                    return carpeta.b_content[k].b_inodo;
                                }else if((cont2 != cont - 1) && (strcasecmp(carpeta.b_content[k].b_name,lista.at(cont2).c_str()) == 0)){
                                    numInodo = byteInodoBloque(stream,carpeta.b_content[k].b_inodo,'1');
                                    siguiente = 1;
                                    break;
                                }
                            }
                            if(siguiente == 1)
                                break;
                        }
                    }
                }else if(i == 13){

                }else if(i == 14){

                }
                if(siguiente == 1)
                    break;
            }
        }
    }

    return -1;
}

bool MV::permisosLecturaRecursivo(FILE* stream, int n){
    SuperBloque super;
    InodoTable inodo;

    fseek(stream,daLoguer.inicioSuper,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,stream);
    fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*n,SEEK_SET);
    fread(&inodo,sizeof(InodoTable),1,stream);

    bool permisos = permisosDeLectura(inodo.i_perm,(inodo.i_uid == daLoguer.id_user),(inodo.i_gid == daLoguer.id_grp));
    if(permisos){
        bool response = true;
        if(inodo.i_type == '0'){//carpeta
            for (int i = 0; i < 12; i++) {//Solo apuntadores directos-falta revisar indirectos
                if(inodo.i_block[i] != -1){
                    char byte = '0';
                    fseek(stream,super.s_bm_block_start + inodo.i_block[i],SEEK_SET);
                    byte = static_cast<char>(fgetc(stream));
                    if(byte == '1'){//Carpeta
                        BloqueCarpeta carpeta;
                        fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*inodo.i_block[i],SEEK_SET);
                        fread(&carpeta,sizeof(BloqueCarpeta),1,stream);
                        for (int j = 0; j < 4; j++) {
                            if(carpeta.b_content[j].b_inodo != -1){
                                if(strcmp(carpeta.b_content[j].b_name,".")!=0 && strcmp(carpeta.b_content[j].b_name,"..")!=0)
                                    response = permisosLecturaRecursivo(stream,carpeta.b_content[j].b_inodo);
                            }
                        }
                    }else{//Archivo
                        InodoTable inodoAux;
                        fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*inodo.i_block[i],SEEK_SET);
                        fread(&inodoAux,sizeof(InodoTable),1,stream);
                        response = permisosDeLectura(inodoAux.i_perm,(inodoAux.i_uid == daLoguer.id_user),(inodoAux.i_gid == daLoguer.id_grp));
                    }
                }
            }
            return response;
        }else //archivo
            return true;
    }else
        return false;
}

bool MV::permisosDeLectura(int permisos, bool flagUser, bool flagGroup){
    string aux = to_string(permisos);
    int propietario = aux[0] - '0';
    int grupo = aux[1] - '0';
    int otros = aux[2] - '0';

    if((propietario >= 3) && flagUser)
        return true;
    else if((grupo >= 3) && flagGroup)
        return true;
    else if(otros >= 3)
        return true;

    return false;
}

bool MV::permisosDeEscritura(int permisos, bool flagUser, bool flagGroup){
    string aux = to_string(permisos);
    char propietario = aux[0];
    char grupo = aux[1];
    char otros = aux[2];

    if((propietario == '2' || propietario == '3' || propietario == '6' || propietario || '7') && flagUser)
        return true;
    else if((grupo == '2' || grupo == '3' || grupo == '6' || grupo == '7') && flagGroup)
        return true;
    else if(otros == '2' || otros == '3' || otros == '6' || otros == '7')
        return true;

    return false;
}

void MV::moverCarpetaArchivo(FILE *stream, int carpeta, char* path,int destino){
    SuperBloque super;
    InodoTable inodo;

    fseek(stream,daLoguer.inicioSuper,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,stream);
    fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*carpeta,SEEK_SET);
    fread(&inodo,sizeof(InodoTable),1,stream);

    if(inodo.i_type == '0'){
        BloqueCarpeta carp;
        fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*inodo.i_block[0],SEEK_SET);
        fread(&carp,sizeof(BloqueCarpeta),1,stream);
        carp.b_content[1].b_inodo = destino;
        fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*inodo.i_block[0],SEEK_SET);
        fwrite(&carp,sizeof(BloqueCarpeta),1,stream);
    }

    BloqueCarpeta carpet;
    //Bloque y posicion de la carpeta/archivo a mover
    int bloque = 0;
    int posicion = 0;
    int pointer = -1;
    int posPointer = 0;
    bloqueCarpetaArchivo(stream,path,bloque,posicion,pointer,posPointer);
    //Nombre de la carpeta/archivo a mover y el inodo al que apunta
    char tempNombre[15];
    int aux = 0;
    bool flag = false;

    fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*bloque,SEEK_SET);
    fread(&carpet,sizeof(BloqueCarpeta),1,stream);
    strcpy(tempNombre,carpet.b_content[posicion].b_name);
    aux = carpet.b_content[posicion].b_inodo;
    //Borramos la referencia de esa carpeta/archivo
    memset(carpet.b_content[posicion].b_name,0,sizeof(carpet.b_content[posicion].b_name));
    carpet.b_content[posicion].b_inodo = -1;
    fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*bloque,SEEK_SET);
    fwrite(&carpet,sizeof(BloqueCarpeta),1,stream);

    fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*destino,SEEK_SET);
    fread(&inodo,sizeof(InodoTable),1,stream);

    for(int i = 0; i < 12; i++){
        if(inodo.i_block[i] != -1){
            BloqueCarpeta carp;
            char byte = '0';
            fseek(stream,super.s_bm_block_start + inodo.i_block[i],SEEK_SET);
            byte = static_cast<char>(fgetc(stream));
            if(byte == '1'){
                fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*inodo.i_block[i],SEEK_SET);
                fread(&carp,sizeof(BloqueCarpeta),1,stream);
                for(int j = 0; j < 4; j++){
                    if(carp.b_content[j].b_inodo == -1){
                        strcpy(carp.b_content[j].b_name,tempNombre);
                        carp.b_content[j].b_inodo = aux;
                        fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*inodo.i_block[i],SEEK_SET);
                        fwrite(&carp,sizeof(BloqueCarpeta),1,stream);
                        flag = true;
                        cout << "Carpeta/archivo movido con exito" << endl;
                        break;
                    }
                }
            }
            if(flag)
                break;
        }else{//Se tiene que crear una carpeta
            char buffer = '1';
            //Guardamos registramos el bloque en el inodo
            int bitBloque = buscarBit(stream,'B',daLoguer.fit);
            inodo.i_block[i] = bitBloque;
            fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*destino,SEEK_SET);
            fwrite(&inodo,sizeof(InodoTable),1,stream);
            //Creamos la nueva carpeta
            BloqueCarpeta nuevaCarpeta;
            nuevaCarpeta.b_content[0].b_inodo = aux;
            nuevaCarpeta.b_content[1].b_inodo = -1;
            nuevaCarpeta.b_content[2].b_inodo = -1;
            nuevaCarpeta.b_content[3].b_inodo = -1;
            strcpy(nuevaCarpeta.b_content[0].b_name,tempNombre);
            strcpy(nuevaCarpeta.b_content[1].b_name,"");
            strcpy(nuevaCarpeta.b_content[2].b_name,"");
            strcpy(nuevaCarpeta.b_content[3].b_name,"");
            fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*bitBloque,SEEK_SET);
            fwrite(&nuevaCarpeta,sizeof(BloqueCarpeta),1,stream);
            //Registramos el bloque en el bitmap
            fseek(stream,super.s_bm_block_start + bitBloque,SEEK_SET);
            fwrite(&buffer,sizeof(char),1,stream);
            //Modificamos el superbloque
            super.s_free_blocks_count = super.s_free_blocks_count - 1;
            super.s_first_blo = super.s_first_blo + 1;
            fseek(stream,daLoguer.inicioSuper,SEEK_SET);
            fwrite(&super,sizeof(SuperBloque),1,stream);
            cout << "Carpeta/archivo movido con exito" << endl;
            char aux[500];
            char operacion[8];
            string datos = "Ruta: "+this->path + ", Destino: " + this->dest;
            strcpy(aux,datos.c_str());
            strcpy(operacion,"MV");
            guardarJournal(operacion,1,664,aux);
            break;
        }
    }

}

int MV::buscarBit(FILE *fp, char tipo, char fit){
    SuperBloque super;
    int inicio_bm = 0;
    char tempBit = '0';
    int bit_libre = -1;
    int tam_bm = 0;

    fseek(fp,daLoguer.inicioSuper,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,fp);

    if(tipo == 'I'){
        tam_bm = super.s_inodes_count;
        inicio_bm = super.s_bm_inode_start;
    }else if(tipo == 'B'){
        tam_bm = super.s_blocks_count;
        inicio_bm = super.s_bm_block_start;
    }

    /*----------------Tipo de ajuste a utilizar----------------*/
    if(fit == 'f' || fit == 'F'){//Primer ajuste
        for(int i = 0; i < tam_bm; i++){
            fseek(fp,inicio_bm + i,SEEK_SET);
            tempBit = static_cast<char>(fgetc(fp));
            if(tempBit == '0'){
                bit_libre = i;
                return bit_libre;
            }
        }

        if(bit_libre == -1)
            return -1;

    }else if(fit == 'b' || fit == 'B'){//Mejor ajuste
        int libres = 0;
        int auxLibres = -1;

        for(int i = 0; i < tam_bm; i++){//Primer recorrido
            fseek(fp,inicio_bm + i,SEEK_SET);
            tempBit = static_cast<char>(fgetc(fp));
            if(tempBit == '0'){
                libres++;
                if(i+1 == tam_bm){
                    if(auxLibres == -1 || auxLibres == 0)
                        auxLibres = libres;
                    else{
                        if(auxLibres > libres)
                            auxLibres = libres;
                    }
                    libres = 0;
                }
            }else if(tempBit == '1'){
                if(auxLibres == -1 || auxLibres == 0)
                    auxLibres = libres;
                else{
                    if(auxLibres > libres)
                        auxLibres = libres;
                }
                libres = 0;
            }
        }

        for(int i = 0; i < tam_bm; i++){
            fseek(fp,inicio_bm + i, SEEK_SET);
            tempBit = static_cast<char>(fgetc(fp));
            if(tempBit == '0'){
                libres++;
                if(i+1 == tam_bm)
                    return ((i+1)-libres);
            }else if(tempBit == '1'){
                if(auxLibres == libres)
                    return ((i+1) - libres);
                libres = 0;
            }
        }

        return -1;

    }else if(fit == 'w' || fit == 'W'){//Peor ajuste
        int libres = 0;
        int auxLibres = -1;

        for (int i = 0; i < tam_bm; i++) {//Primer recorrido
            fseek(fp,inicio_bm + i, SEEK_SET);
            tempBit = static_cast<char>(fgetc(fp));
            if(tempBit == '0'){
                libres++;
                if(i+1 == tam_bm){
                    if(auxLibres == -1 || auxLibres == 0)
                        auxLibres = libres;
                    else {
                        if(auxLibres < libres)
                            auxLibres = libres;
                    }
                    libres = 0;
                }
            }else if(tempBit == '1'){
                if(auxLibres == -1 || auxLibres == 0)
                    auxLibres = libres;
                else{
                    if(auxLibres < libres)
                        auxLibres = libres;
                }
                libres = 0;
            }
        }

        for (int i = 0; i < tam_bm; i++) {
            fseek(fp,inicio_bm + i, SEEK_SET);
            tempBit = static_cast<char>(fgetc(fp));
            if(tempBit == '0'){
                libres++;
                if(i+1 == tam_bm)
                    return ((i+1) - libres);
            }else if(tempBit == '1'){
                if(auxLibres == libres)
                    return ((i+1) - libres);
                libres = 0;
            }
        }

        return -1;
    }

    return 0;
}

void MV::bloqueCarpetaArchivo(FILE *stream, char* path, int &block, int &posicion,int &pointer,int &posPointer){
    SuperBloque super;
    InodoTable inodo;
    BloqueCarpeta carpeta;
    BloqueApuntadores apuntador;

    QList<string> lista = QList<string>();
    char *token = strtok(path,"/");
    int cont = 0;
    int numInodo = 0;

    while(token != nullptr){
        lista.append(token);
        cont++;
        token = strtok(nullptr,"/");
    }

    fseek(stream,daLoguer.inicioSuper,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,stream);
    numInodo = super.s_inode_start;//Inodo 0 '/'

    for (int cont2 = 0; cont2 < cont; cont2++) {
        fseek(stream,numInodo,SEEK_SET);
        fread(&inodo,sizeof(InodoTable),1,stream);
        int siguiente = 0;
        for(int i = 0; i < 12; i++){
            if(inodo.i_block[i] != -1){
                int byteBloque = byteInodoBloque(stream,inodo.i_block[i],'2');
                fseek(stream,byteBloque,SEEK_SET);
                if(i < 12){
                    fread(&carpeta,sizeof(BloqueCarpeta),1,stream);
                    for (int j = 0; j < 4; j++) {
                        if((cont2 == cont - 1) && (strcasecmp(carpeta.b_content[j].b_name,lista.at(cont2).c_str()) == 0)){//Tendria que ser la carpeta
                            block = inodo.i_block[i];
                            posicion = j;
                        }else if((cont2 != cont - 1) && (strcasecmp(carpeta.b_content[j].b_name,lista.at(cont2).c_str()) == 0)){
                            numInodo = byteInodoBloque(stream,carpeta.b_content[j].b_inodo,'1');
                            siguiente = 1;
                            break;
                        }
                    }
                }else if(i == 12){
                    fread(&apuntador,sizeof(BloqueApuntadores),1,stream);
                    for(int j = 0; j < 16; j++){
                        if(apuntador.b_pointer[j] != -1){
                            byteBloque = byteInodoBloque(stream,apuntador.b_pointer[j],'2');
                            fseek(stream,byteBloque,SEEK_SET);
                            fread(&carpeta,sizeof(BloqueCarpeta),1,stream);
                            for (int k = 0; k < 4; k++) {
                                if((cont2 == cont - 1) && (strcasecmp(carpeta.b_content[k].b_name,lista.at(cont2).c_str()) == 0)){//Tendria que ser la carpeta
                                    pointer = inodo.i_block[i];
                                    posPointer = j;
                                    block = apuntador.b_pointer[j];
                                    posicion = k;
                                }else if((cont2 != cont - 1) && (strcasecmp(carpeta.b_content[k].b_name,lista.at(cont2).c_str()) == 0)){
                                    numInodo = byteInodoBloque(stream,carpeta.b_content[k].b_inodo,'1');
                                    siguiente = 1;
                                    break;
                                }
                            }
                            if(siguiente == 1)
                                break;
                        }
                    }
                }
                if(siguiente == 1)
                    break;
            }
        }
    }
}
