#include "EDIT.h"

EDIT::EDIT(string path, string cont, bool move, bool copy)
{
    this->path = path;
    this->cont = cont;
    this->move = move;
    this->copy = copy;
}

void EDIT::Ejecutar(){
    if(login){
        char auxPath[500];
        strcpy(auxPath,path.c_str());
        FILE *fp = fopen(daLoguer.direccion.c_str(),"r+b");
        int carpeta = buscarCarpetaArchivo(fp,auxPath);
        if(carpeta != -1){
            SuperBloque super;
            InodoTable inodo;
            string cadena = "";
            fseek(fp,daLoguer.inicioSuper,SEEK_SET);
            fread(&super,sizeof(SuperBloque),1,fp);
            fseek(fp,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*carpeta,SEEK_SET);
            fread(&inodo,sizeof(InodoTable),1,fp);
            bool permisos = permisosDeLectura(inodo.i_perm,(inodo.i_uid == daLoguer.id_user),(inodo.i_gid == daLoguer.id_grp));
            bool escritura = permisosDeEscritura(inodo.i_perm,(inodo.i_uid == daLoguer.id_user),(inodo.i_gid == daLoguer.id_grp));
            if(permisos || (daLoguer.id_user == 1 && daLoguer.id_grp == 1)){
                if(escritura || (daLoguer.id_user == 1 && daLoguer.id_grp == 1)){
                    for (int i = 0; i < 15; i++) {
                        if(inodo.i_block[i] != -1){
                            if(i < 12){
                                BloqueArchivo archivo;
                                fseek(fp,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*inodo.i_block[i],SEEK_SET);
                                fread(&archivo,sizeof(BloqueCarpeta),1,fp);
                                cadena += archivo.b_content;
                            }else if(i == 12){//Apuntador indirecto simple
                                BloqueApuntadores apuntador;
                                fseek(fp,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*inodo.i_block[i],SEEK_SET);
                                fread(&apuntador,sizeof(BloqueApuntadores),1,fp);
                                for(int j = 0; j < 16; j++){
                                    if(apuntador.b_pointer[j] != -1){
                                        BloqueArchivo archivo;
                                        fseek(fp,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*apuntador.b_pointer[j],SEEK_SET);
                                        fread(&archivo,sizeof(BloqueArchivo),1,fp);
                                        cadena += archivo.b_content;
                                    }

                                }
                            }
                        }
                    }
                    fclose(fp);
                    if(!move && !copy){
                        string arreglo = cadena + cont;
                        REM *rm = new REM(this->path, true);
                        rm->Ejecutar();
                        MKFILE *mkfile = new MKFILE(this->path, true, (int) arreglo.length(), "", true, arreglo);
                        mkfile->Ejecutar();
                        arreglo.clear();
                        delete(mkfile);
                        cout << "Se edito el archivo" << endl;
                        char aux[500];
                        char operacion[8];
                        string datos = "Ruta: "+this->path + ", Cont: " + cont;
                        strcpy(aux,datos.c_str());
                        strcpy(operacion,"EDIT");
                        guardarJournal(operacion,1,664,aux);
                    }else if(copy && !move){
                        string s = this->path;
                        string delimiter = "/";
                        size_t pos = 0;
                        std::string token;
                        while ((pos = s.find(delimiter)) != std::string::npos) {
                            token = s.substr(0, pos);
                            s.erase(0, pos + delimiter.length());
                        }
                        string carpeta = cont + +"/"+s;
                        if(cont.find(".")!=string::npos){

                            MKFILE *mkfile = new MKFILE(carpeta, true, (int) cadena.length(), "", true, cadena);
                            mkfile->Ejecutar();
                        }else{
                            MKDir *mkdir = new MKDir(carpeta, true);
                            mkdir->Ejecutar();
                        }
                        cout << "Se copio el carpeta/archivo" << endl;
                        char aux[500];
                        char operacion[8];
                        string datos = "Ruta: "+this->path + ", Destino: " + cont;
                        strcpy(aux,datos.c_str());
                        strcpy(operacion,"CP");
                        guardarJournal(operacion,1,664,aux);
                    }


                }else{
                    cout << "El usuario no tiene permisos de escritura" << endl;
                    fclose(fp);
                }
            }else{
                cout << "El usuario no tiene permisos de lectura" << endl;
                fclose(fp);
            }
        }else{
            cout << "No se encuentra el archivo " << path << endl;
            fclose(fp);
        }
    }else{
        cout << "Debe iniciar sesion para usar el comando" << endl;

    }
}

void EDIT::guardarJournal(char *operacion, int tipo, int permisos, char *nombre){
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


int EDIT::byteInodoBloque(FILE *stream,int pos, char tipo){
    SuperBloque super;
    fseek(stream,daLoguer.inicioSuper,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,stream);
    if(tipo == '1'){
        return (super.s_inode_start + static_cast<int>(sizeof(InodoTable))*pos);
    }else if(tipo == '2')
        return (super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*pos);
    return 0;
}

int EDIT::buscarCarpetaArchivo(FILE *stream, char* path){
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

bool EDIT::permisosDeLectura(int permisos, bool flagUser, bool flagGroup){
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

bool EDIT::permisosDeEscritura(int permisos, bool flagUser, bool flagGroup){
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
