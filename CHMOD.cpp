#include "CHMOD.h"

CHMOD::CHMOD(string path, int ugo, bool recursivo)
{
    this->path = path;
    this->ugo = ugo;
    this->recursivo = recursivo;
}

void CHMOD::Ejecutar(){
    if(login){
        if(daLoguer.id_user == 1 && daLoguer.id_grp == 1){
            string permiso = to_string(ugo);
            int propietario = permiso[0];
            int grupo = permiso[1];
            int otros = permiso[2];
            if((propietario >= 0 && propietario <= 7) && (grupo >= 0 && grupo <= 7) && (otros >= 0 && otros <= 7)){
                char auxPath[500];
                strcpy(auxPath,path.c_str());
                FILE *fp = fopen(daLoguer.direccion.c_str(),"r+b");
                SuperBloque super;
                InodoTable inodo;
                int existe = buscarCarpetaArchivo(fp,auxPath);
                fseek(fp,daLoguer.inicioSuper,SEEK_SET);
                fread(&super,sizeof(SuperBloque),1,fp);
                fseek(fp,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*existe,SEEK_SET);
                fread(&inodo,sizeof(InodoTable),1,fp);
                if(existe != -1){
                    if((daLoguer.id_user ==1 && daLoguer.id_grp == 1) || daLoguer.id_user == inodo.i_uid){
                        if(recursivo)
                            cambiarPermisosRecursivo(fp,existe,ugo);
                        else{
                            inodo.i_perm = ugo;
                            fseek(fp,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*existe,SEEK_SET);
                            fwrite(&inodo,sizeof(InodoTable),1,fp);
                        }
                        cout << "Permisos cambiados exitosamente" << endl;
                    }else{
                        cout << "Para cambiar los permisos debe ser el usuario root o ser dueño de la carpeta/archivo" << endl;
                    }
                }else{
                    cout << "La ruta no existe" << endl;
                }
                fclose(fp);
            }else{
                cout << "El permiso debe ser entre 0 y 7"<< endl;
            }
        }else{
            cout << "Debe ser usuario root para usar el comando" << endl;
        }
    }else{
        cout << "Debe iniciar sesion para usar el comando" << endl;
    }
}

int CHMOD::buscarCarpetaArchivo(FILE *stream, char* path){
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

void CHMOD::cambiarPermisosRecursivo(FILE* stream, int n, int permisos){
    SuperBloque super;
    InodoTable inodo;
    BloqueCarpeta carpeta;
    char byte ='0';

    fseek(stream,daLoguer.inicioSuper,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,stream);
    fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*n,SEEK_SET);
    fread(&inodo,sizeof(InodoTable),1,stream);
    inodo.i_perm = permisos;
    fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*n,SEEK_SET);
    fwrite(&inodo,sizeof(InodoTable),1,stream);

    for(int i = 0; i < 15; i++){
        if(inodo.i_block[i] != -1){
            fseek(stream,super.s_bm_block_start + inodo.i_block[i],SEEK_SET);
            byte = static_cast<char>(fgetc(stream));
            if(byte == '1'){
                fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*inodo.i_block[i],SEEK_SET);
                fread(&carpeta,sizeof(BloqueCarpeta),1,stream);
                for(int j = 0; j < 4; j++){
                    if(carpeta.b_content[j].b_inodo != -1){
                        if(strcmp(carpeta.b_content[j].b_name,".")!=0 &&  strcmp(carpeta.b_content[j].b_name,"..")!=0)
                            cambiarPermisosRecursivo(stream,carpeta.b_content[j].b_inodo,permisos);
                    }
                }
            }
        }
    }
}

int CHMOD::byteInodoBloque(FILE *stream,int pos, char tipo){
    SuperBloque super;
    fseek(stream,daLoguer.inicioSuper,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,stream);
    if(tipo == '1'){
        return (super.s_inode_start + static_cast<int>(sizeof(InodoTable))*pos);
    }else if(tipo == '2')
        return (super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*pos);
    return 0;
}
