#include "REM.h"

REM::REM(string path)
{
    this->path = path;
}

void REM::Ejecutar(){
    if(login){
        char auxPath[500];
        strcpy(auxPath,path.c_str());
        FILE *fp = fopen(daLoguer.direccion.c_str(),"r+b");
        int existe = buscarCarpetaArchivo(fp,auxPath);
        if(existe != -1){
            bool permisos = permisosEscrituraRecursivo(fp,existe);
            if(permisos){
                SuperBloque super;
                BloqueCarpeta carpeta;
                BloqueApuntadores apuntador;
                int bloque = 0;
                int posicion = 0;
                int pointer = -1;
                int posPointer = 0;
                fseek(fp,daLoguer.inicioSuper,SEEK_SET);
                fread(&super,sizeof(SuperBloque),1,fp);
                //Obtenemos el bloque y la poisicion de la carpeta/archivo a eliminar
                bloqueCarpetaArchivo(fp,auxPath,bloque,posicion,pointer,posPointer);
                if(pointer == -1){
                    //Eliminar la referencia carpeta/archivo del bloque carpeta
                    fseek(fp,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*bloque,SEEK_SET);
                    fread(&carpeta,sizeof(BloqueCarpeta),1,fp);
                    memset(carpeta.b_content[posicion].b_name,0,sizeof(carpeta.b_content[posicion].b_name));
                    carpeta.b_content[posicion].b_inodo = -1;
                    fseek(fp,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*bloque,SEEK_SET);
                    fwrite(&carpeta,sizeof(BloqueCarpeta),1,fp);
                }else{
                    fseek(fp,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*pointer,SEEK_SET);
                    fread(&apuntador,sizeof(BloqueApuntadores),1,fp);
                    apuntador.b_pointer[posPointer] = -1;
                    fseek(fp,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*pointer,SEEK_SET);
                    fwrite(&apuntador,sizeof(BloqueApuntadores),1,fp);
                }
                //Eliminar la carpeta/archivo
                eliminarCarpetaArchivo(fp,existe);
                cout << "Removido con exito" << endl;
            }else
                cout << "ERROR: alguna carpeta hija no posee permisos de escritura" << endl;
        }else
            cout << "ERROR: no se encuentra la direccion" << endl;
        fclose(fp);
    }else{
        cout << "Debe iniciar sesion para usar el comando" << endl;
    }
}

int REM::byteInodoBloque(FILE *stream,int pos, char tipo){
    SuperBloque super;
    fseek(stream,daLoguer.inicioSuper,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,stream);
    if(tipo == '1'){
        return (super.s_inode_start + static_cast<int>(sizeof(InodoTable))*pos);
    }else if(tipo == '2')
        return (super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*pos);
    return 0;
}

int REM::buscarCarpetaArchivo(FILE *stream, char* path){
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

bool REM::permisosEscrituraRecursivo(FILE* stream, int n){
    SuperBloque super;
    InodoTable inodo;

    fseek(stream,daLoguer.inicioSuper,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,stream);
    fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*n,SEEK_SET);
    fread(&inodo,sizeof(InodoTable),1,stream);

    bool permisos = permisosDeEscritura(inodo.i_perm,(inodo.i_uid == daLoguer.id_user),(inodo.i_gid == daLoguer.id_grp));
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
                                    response = permisosEscrituraRecursivo(stream,carpeta.b_content[j].b_inodo);
                            }
                        }
                    }else{//Archivo
                        InodoTable inodoAux;
                        fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*inodo.i_block[i],SEEK_SET);
                        fread(&inodoAux,sizeof(InodoTable),1,stream);
                        response = permisosDeEscritura(inodoAux.i_perm,(inodoAux.i_uid == daLoguer.id_user),(inodoAux.i_gid == daLoguer.id_grp));
                    }
                }
            }
            return response;
        }else //archivo
            return true;
    }else
        return false;
}

bool REM::permisosDeEscritura(int permisos, bool flagUser, bool flagGroup){
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

void REM::bloqueCarpetaArchivo(FILE *stream, char* path, int &block, int &posicion,int &pointer,int &posPointer){
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

void REM::eliminarCarpetaArchivo(FILE *stream, int n){
    SuperBloque super;
    InodoTable inodo;
    char buffer = '0';

    fseek(stream,daLoguer.inicioSuper,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,stream);
    fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*n,SEEK_SET);
    fread(&inodo,sizeof(InodoTable),1,stream);

    if(inodo.i_type == '0'){//Carpeta
        for (int i = 0; i < 12; i++) {
            if(inodo.i_block[i] != -1){
                char byte = '0';
                fseek(stream,super.s_bm_block_start + inodo.i_block[i],SEEK_SET);
                byte = static_cast<char>(fgetc(stream));
                if(byte == '1'){//Bloque carpeta
                    BloqueCarpeta carpeta;
                    fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*inodo.i_block[i],SEEK_SET);
                    fread(&carpeta,sizeof(BloqueCarpeta),1,stream);
                    for (int j = 0; j < 4; j++) {
                        if(carpeta.b_content[j].b_inodo != -1){
                            if(strcmp(carpeta.b_content[j].b_name,".")!=0 && strcmp(carpeta.b_content[j].b_name,"..")!=0)
                                eliminarCarpetaArchivo(stream,carpeta.b_content[j].b_inodo);
                        }
                    }
                    fseek(stream,super.s_bm_block_start + inodo.i_block[i],SEEK_SET);
                    fputc(buffer,stream);
                }
            }
        }
        //Eliminar inodo del bitmap
        fseek(stream,super.s_bm_inode_start + n,SEEK_SET);
        fputc(buffer,stream);
    }else{//Archivo
        //Eliminar bloques del bitmap
        for (int i = 0; i < 15; i++) {
            if(inodo.i_block[i] != -1){
                fseek(stream,super.s_bm_block_start + inodo.i_block[i],SEEK_SET);
                fputc(buffer,stream);
            }
        }
        //Eliminar inodo del bitmap
        fseek(stream,super.s_bm_inode_start + n,SEEK_SET);
        fputc(buffer,stream);
    }

}
