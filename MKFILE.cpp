#include "MKFILE.h"

MKFILE::MKFILE(string path, bool p, int size, string cont)
{
    this->path = path;
    this->p = p;
    this->size = size;
    this->cont = cont;
}

void MKFILE::Ejecutar(){
    if(login){
        QFileInfo fileName(this->path.c_str());
        QString name = fileName.fileName();
        if(name.length() <= 11){
            int resultado = crearArchivo(this->path, this->p, this->size, this->cont);
            if(resultado == 1){
                if(daLoguer.tipo_sistema == 3){
                    char aux[500];
                    char operacion[8];
                    string padre="";
                    if(this->p){
                        padre = "Si";
                    }else{
                        padre = "No";
                    }
                    string datos = "Ruta: "+this->path+" P: "+padre+" Size: "+to_string(this->size)+" Cont: "+this->cont;
                    strcpy(aux,datos.c_str());
                    strcpy(operacion,"mkfile");
                    guardarJournal(operacion,1,664,aux);
                }
                cout << "Archivo creado con exito" << endl;
            }else if(resultado == 2){
                cout << "El usuario no tiene permisos de escritura" << endl;
            }else if(resultado == 3){
                cout << "El archivo contenido no existe" << endl;
            }else if(resultado == 4){
                cout << "No existe la ruta y no coloco el parametro -P" << endl;
            }
        }else{
            cout << "El nombre del archivo es mas grande que lo esperado" << endl;
        }
    }else{
        cout << "Debe iniciar sesion para usar el comando" << endl;
    }
}

void MKFILE::guardarJournal(char *operacion, int tipo, int permisos, char *nombre){
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

int MKFILE::crearArchivo(string path, bool p, int size, string cont){
    FILE *fp = fopen(daLoguer.direccion.c_str(),"r+b");

    SuperBloque super;
    fseek(fp,daLoguer.inicioSuper,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,fp);

    char auxPath[500];
    char auxPath2[500];
    strcpy(auxPath,path.c_str());
    strcpy(auxPath2,path.c_str());
    int existe = buscarCarpetaArchivo(fp,auxPath);
    strcpy(auxPath,path.c_str());
    int response = -1;

    if(existe != -1)
        response = 0;
    else
        response = nuevoArchivo(fp,daLoguer.fit,p,auxPath,size,cont,0,auxPath2);

    fclose(fp);
    return response;
}

int MKFILE::byteInodoBloque(FILE *stream,int pos, char tipo){
    SuperBloque super;
    fseek(stream,daLoguer.inicioSuper,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,stream);
    if(tipo == '1'){
        return (super.s_inode_start + static_cast<int>(sizeof(InodoTable))*pos);
    }else if(tipo == '2')
        return (super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*pos);
    return 0;
}

int MKFILE::buscarCarpetaArchivo(FILE *stream, char* path){
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

int MKFILE::buscarContentLibre(FILE* stream,int numInodo,InodoTable &inodo,BloqueCarpeta &carpeta, BloqueApuntadores &apuntadores,int &content,int &bloque,int &pointer){
    int libre = 0;
    SuperBloque super;
    fseek(stream,daLoguer.inicioSuper,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,stream);
    fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*numInodo,SEEK_SET);
    fread(&inodo,sizeof(InodoTable),1,stream);
    //Buscamos un espacio libre en el bloque carpeta
    for(int i = 0; i < 15; i++){
        if(inodo.i_block[i] != -1){
            if(i == 12){//Apuntador indirecto simple
                fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueApuntadores))*inodo.i_block[i],SEEK_SET);
                fread(&apuntadores,sizeof(BloqueApuntadores),1,stream);
                for(int j = 0; j < 16; j++){
                    if(apuntadores.b_pointer[j] != -1){
                        fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueApuntadores))*apuntadores.b_pointer[j],SEEK_SET);
                        fread(&carpeta,sizeof(BloqueCarpeta),1,stream);
                        for(int k = 0; k < 4; k++){
                            if(carpeta.b_content[k].b_inodo == -1){
                                libre = 1;
                                bloque = i;
                                pointer = j;
                                content = k;
                                break;
                            }
                        }
                    }
                    if(libre == 1)
                        break;
                }
            }else if(i == 13){//Apuntador indirecto doble

            }else if(i == 14){//Apuntador indirecto triple

            }else{//Apuntadores directos
                fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*inodo.i_block[i],SEEK_SET);
                fread(&carpeta,sizeof(BloqueCarpeta),1,stream);
                for(int j = 0; j < 4; j++){
                    if(carpeta.b_content[j].b_inodo == -1){
                        libre = 1;
                        bloque = i;
                        content = j;
                        break;
                    }
                }
            }
        }
        if(libre == 1)
            break;
    }

    return libre;
}

bool MKFILE::permisosDeEscritura(int permisos, bool flagUser, bool flagGroup){
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

int MKFILE::nuevoArchivo(FILE *stream, char fit, bool flagP, char *path, int size, string contenido, int index,char *auxPath){
    SuperBloque super;
    InodoTable inodo,inodoNuevo;
    BloqueCarpeta carpeta, carpetaNueva;
    BloqueApuntadores apuntadores;
    QList<string> lista = QList<string>();
    char copiaPath[500];
    char directorio[500];
    char nombreCarpeta[80];
    string content = "";
    string contentSize = "0123456789";

    strcpy(copiaPath,path);
    strcpy(directorio,dirname(copiaPath));
    strcpy(copiaPath,path);
    strcpy(nombreCarpeta,basename(copiaPath));
    strcpy(copiaPath,path);
    char *token = strtok(path,"/");
    int cont = 0;
    int numInodo = index;
    int finalSize = size;

    while(token != nullptr){
        cont++;
        lista.append(token);
        token = strtok(nullptr,"/");
    }

    if(contenido.length() != 0){
        FILE *archivoCont;
        if((archivoCont = fopen(contenido.c_str(),"r"))){
            fseek(archivoCont,0,SEEK_END);
            finalSize = static_cast<int>(ftell(archivoCont));
            fseek(archivoCont,0,SEEK_SET);
            for (int i = 0; i < finalSize; i++)
                content += static_cast<char>(fgetc(archivoCont));
        }else
            return 3;
    }

    fseek(stream,daLoguer.inicioSuper,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,stream);

    if(cont == 1){
        int bloque = 0;
        int pointer = 0;
        int b_content = 0;
        int libre = buscarContentLibre(stream,numInodo,inodo,carpeta,apuntadores,b_content,bloque,pointer);

        if(libre == 1){
            bool permisos = permisosDeEscritura(inodo.i_perm,(inodo.i_uid == daLoguer.id_user),(inodo.i_gid == daLoguer.id_grp));
            if(permisos || (daLoguer.id_user == 1 && daLoguer.id_grp == 1)){
                char buffer = '1';
                char buffer2 = '2';
                char buffer3 = '3';
                //Agregamos el archivo al bloque correspondiente
                int bitInodo = buscarBit(stream,'I',fit);
                carpeta.b_content[b_content].b_inodo = bitInodo;
                strcpy(carpeta.b_content[b_content].b_name,nombreCarpeta);
                fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*inodo.i_block[bloque],SEEK_SET);
                fwrite(&carpeta,sizeof(BloqueCarpeta),1,stream);
                //Creamos el nuevo inodo archivo
                inodoNuevo = crearInodo(0,'1',664);
                fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*bitInodo,SEEK_SET);
                fwrite(&inodoNuevo,sizeof(InodoTable),1,stream);
                //Registramos el inodo en el bitmap
                fseek(stream,super.s_bm_inode_start + bitInodo,SEEK_SET);
                fwrite(&buffer,sizeof(char),1,stream);
                //Si viene el parametro -size/-cont
                if(finalSize != 0){
                    double n = static_cast<double>(finalSize)/static_cast<double>(63);
                    int numBloques = static_cast<int>(ceil(n));
                    int caracteres = finalSize;
                    size_t charNum = 0;
                    size_t contChar = 0;
                    numInodo = buscarCarpetaArchivo(stream,auxPath);
                    for (int i = 0; i < numBloques; i++) {
                        BloqueArchivo archivo;
                        memset(archivo.b_content,0,sizeof(archivo.b_content));
                        if(i == 12){
                            int bitBloqueA = buscarBit(stream,'B',fit);//Apuntador
                            //Guardamos el bloque en el inodo archivo
                            fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*numInodo,SEEK_SET);
                            fread(&inodo,sizeof(InodoTable),1,stream);
                            inodo.i_block[i] = bitBloqueA;
                            fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*numInodo,SEEK_SET);
                            fwrite(&inodo,sizeof(InodoTable),1,stream);
                            //Registramos el bloque apuntador en el bitmap
                            fseek(stream,super.s_bm_block_start + bitBloqueA,SEEK_SET);
                            fwrite(&buffer3,sizeof(char),1,stream);
                            //Creamos el bloque de apuntadores como es simple solo se debe crear uno
                            int bitBloque = buscarBit(stream,'B',fit);//Primer bloque archivo
                            apuntadores.b_pointer[0] = bitBloque;
                            for(int i = 1; i < 16; i++)
                                apuntadores.b_pointer[i] = -1;
                            fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueApuntadores))*bitBloqueA,SEEK_SET);
                            fwrite(&apuntadores,sizeof(BloqueApuntadores),1,stream);
                            //Empezamos con la creacion de bloques archivos
                            //Registramos el primer bloque en el bitmap
                            fseek(stream,super.s_bm_block_start + bitBloque,SEEK_SET);
                            fwrite(&buffer2,sizeof(char),1,stream);
                            if(caracteres > 63){
                                for(int j = 0; j < 63; j++){
                                    if(content.length() != 0){//-cont
                                        archivo.b_content[j] = content[contChar];
                                        contChar++;
                                    }else{//-size
                                        archivo.b_content[j] = contentSize[charNum];
                                        charNum++;
                                        if(charNum == 10)
                                            charNum = 0;
                                    }
                                }
                                //Guardamos el bloque
                                fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueArchivo))*bitBloque,SEEK_SET);
                                fwrite(&archivo,sizeof(BloqueArchivo),1,stream);
                                caracteres -= 63;
                            }else{
                                for (int j = 0; j < caracteres; j++) {
                                    if(content.length() != 0){
                                        archivo.b_content[j] = content[contChar];
                                        contChar++;
                                    }else{
                                        archivo.b_content[j] = contentSize[charNum];
                                        charNum++;
                                        if(charNum == 10)
                                            charNum = 0;
                                    }
                                }
                                //Guardamos el bloque
                                fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueArchivo))*bitBloque,SEEK_SET);
                                fwrite(&archivo,sizeof(BloqueArchivo),1,stream);
                            }
                        }else if(i > 12 && i < 28){
                            int libre = 0;
                            fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*numInodo,SEEK_SET);
                            fread(&inodo,sizeof(InodoTable),1,stream);
                            fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueApuntadores))*inodo.i_block[12],SEEK_SET);
                            fread(&apuntadores,sizeof(BloqueApuntadores),1,stream);
                            for (int a = 0; a < 16; a++) {
                                if(apuntadores.b_pointer[a] == -1){
                                    libre = a;
                                    break;
                                }
                            }
                            int bitBloque = buscarBit(stream,'B',fit);
                            apuntadores.b_pointer[libre] = bitBloque;
                            fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueApuntadores))*inodo.i_block[12],SEEK_SET);
                            fwrite(&apuntadores,sizeof(BloqueApuntadores),1,stream);
                            //Registramos el bloque en el bitmap
                            fseek(stream,super.s_bm_block_start + bitBloque,SEEK_SET);
                            fwrite(&buffer2,sizeof(char),1,stream);
                            if(caracteres > 63){
                                for(int j = 0; j < 63; j++){
                                    if(content.length() != 0){//-cont
                                        archivo.b_content[j] = content[contChar];
                                        contChar++;
                                    }else{//-size
                                        archivo.b_content[j] = contentSize[charNum];
                                        charNum++;
                                        if(charNum == 10)
                                            charNum = 0;
                                    }
                                }
                                //Guardamos el bloque
                                fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueArchivo))*bitBloque,SEEK_SET);
                                fwrite(&archivo,sizeof(BloqueArchivo),1,stream);
                                caracteres -= 63;
                            }else{
                                for (int j = 0; j < caracteres; j++) {
                                    if(content.length() != 0){
                                        archivo.b_content[j] = content[contChar];
                                        contChar++;
                                    }else{
                                        archivo.b_content[j] = contentSize[charNum];
                                        charNum++;
                                        if(charNum == 10)
                                            charNum = 0;
                                    }
                                }
                                //Guardamos el bloque
                                fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueArchivo))*bitBloque,SEEK_SET);
                                fwrite(&archivo,sizeof(BloqueArchivo),1,stream);
                            }
                        }else if(i == 29){//Apuntador doble

                        }else{
                            int bitBloque = buscarBit(stream,'B',fit);
                            //Registramos el bloque en el bitmap
                            fseek(stream,super.s_bm_block_start + bitBloque,SEEK_SET);
                            fwrite(&buffer2,sizeof(char),1,stream);
                            if(caracteres > 63){
                                for(int j = 0; j < 63; j++){
                                    if(content.length() != 0){//-cont
                                        archivo.b_content[j] = content[contChar];
                                        contChar++;
                                    }else{//-size
                                        archivo.b_content[j] = contentSize[charNum];
                                        charNum++;
                                        if(charNum == 10)
                                            charNum = 0;
                                    }
                                }
                                //Guardamos el bloque en el respectivo inodo archivo
                                fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*numInodo,SEEK_SET);
                                fread(&inodo,sizeof(InodoTable),1,stream);
                                inodo.i_block[i] = bitBloque;
                                inodo.i_size = finalSize;
                                fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*numInodo,SEEK_SET);
                                fwrite(&inodo,sizeof(InodoTable),1,stream);
                                //Guardamos el bloque
                                fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueArchivo))*bitBloque,SEEK_SET);
                                fwrite(&archivo,sizeof(BloqueArchivo),1,stream);
                                caracteres -= 63;
                            }else{
                                for (int j = 0; j < caracteres; j++) {
                                    if(content.length() != 0){
                                        archivo.b_content[j] = content[contChar];
                                        contChar++;
                                    }else{
                                        archivo.b_content[j] = contentSize[charNum];
                                        charNum++;
                                        if(charNum == 10)
                                            charNum = 0;
                                    }
                                }
                                //Guardamos el bloque en el respectivo inodo archivo
                                fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*numInodo,SEEK_SET);
                                fread(&inodo,sizeof(InodoTable),1,stream);
                                inodo.i_block[i] = bitBloque;
                                inodo.i_size = finalSize;
                                fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*numInodo,SEEK_SET);
                                fwrite(&inodo,sizeof(InodoTable),1,stream);
                                //Guardamos el bloque
                                fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueArchivo))*bitBloque,SEEK_SET);
                                fwrite(&archivo,sizeof(BloqueArchivo),1,stream);
                            }
                        }
                    }
                    //Modificamos el super bloque
                    super.s_free_blocks_count = super.s_free_blocks_count - numBloques;
                    super.s_free_inodes_count = super.s_free_inodes_count - 1;
                    super.s_first_ino = super.s_first_ino + 1;
                    super.s_first_blo = super.s_first_blo + numBloques;
                    fseek(stream,daLoguer.inicioSuper,SEEK_SET);
                    fwrite(&super,sizeof(SuperBloque),1,stream);
                    return 1;
                }
                super.s_free_inodes_count = super.s_free_inodes_count - 1;
                super.s_first_ino = super.s_first_ino + 1;
                fseek(stream,daLoguer.inicioSuper,SEEK_SET);
                fwrite(&super,sizeof(SuperBloque),1,stream);
                return 1;
            }else
                return 2;
        }else{//Todos los bloques estan llenos
            fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*numInodo,SEEK_SET);
            fread(&inodo,sizeof(InodoTable),1,stream);
            for (int i = 0; i < 15; i++) {
                if(inodo.i_block[i] == -1){
                    bloque = i;
                    break;
                }
            }
            if(bloque == 12){//Apuntador simple indirecto
                bool permissions = permisosDeEscritura(inodo.i_perm,(inodo.i_uid == daLoguer.id_user),(inodo.i_gid == daLoguer.id_grp));
                if(permissions || (daLoguer.id_user == 1 && daLoguer.id_grp == 1) ){

                }else
                    return 2;
            }else if(bloque == 13){//Apuntador doble indirecto

            }else if(bloque == 14){//Apuntador triple indirecto

            }else{//Apuntadores directos
                bool permisos = permisosDeEscritura(inodo.i_perm,(inodo.i_uid == daLoguer.id_user),(inodo.i_gid == daLoguer.id_grp));
                if(permisos || (daLoguer.id_user == 1 && daLoguer.id_grp == 1)){
                    char buffer = '1';
                    char buffer2 = '2';
                    char buffer3 = '3';
                    int bitBloque = buscarBit(stream,'B',fit);
                    //Guardamos el bloque en el inodo
                    inodo.i_block[bloque] = bitBloque;
                    fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*numInodo,SEEK_SET);
                    fwrite(&inodo,sizeof(InodoTable),1,stream);
                    //Creamos el nuevo bloque carpeta
                    int bitInodo = buscarBit(stream,'I',fit);
                    carpetaNueva.b_content[0].b_inodo = bitInodo;
                    carpetaNueva.b_content[1].b_inodo = -1;
                    carpetaNueva.b_content[2].b_inodo = -1;
                    carpetaNueva.b_content[3].b_inodo = -1;
                    strcpy(carpetaNueva.b_content[0].b_name,nombreCarpeta);
                    strcpy(carpetaNueva.b_content[1].b_name,"");
                    strcpy(carpetaNueva.b_content[2].b_name,"");
                    strcpy(carpetaNueva.b_content[3].b_name,"");
                    fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*bitBloque,SEEK_SET);
                    fwrite(&carpetaNueva,sizeof(BloqueCarpeta),1,stream);
                    //Registramos el bloque en el bitmap
                    fseek(stream,super.s_bm_block_start + bitBloque,SEEK_SET);
                    fwrite(&buffer,sizeof(char),1,stream);
                    //Creamos el nuevo inodo
                    inodoNuevo = crearInodo(0,'1',664);
                    fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*bitInodo,SEEK_SET);
                    fwrite(&inodoNuevo,sizeof(InodoTable),1,stream);
                    fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*bitInodo,SEEK_SET);
                    fwrite(&inodoNuevo,sizeof(InodoTable),1,stream);
                    //Registramos el inodo en el bitmap
                    fseek(stream,super.s_bm_inode_start + bitInodo,SEEK_SET);
                    fwrite(&buffer,sizeof(char),1,stream);
                    //Si viene el parametro -size/-cont
                    if(finalSize != 0){
                        double n = static_cast<double>(finalSize)/static_cast<double>(63);
                        int numBloques = static_cast<int>(ceil(n));
                        int caracteres = finalSize;
                        size_t charNum = 0;
                        size_t contChar = 0;
                        numInodo = buscarCarpetaArchivo(stream,auxPath);
                        for (int i = 0; i < numBloques; i++) {
                            BloqueArchivo archivo;
                            memset(archivo.b_content,0,sizeof(archivo.b_content));
                            if(i == 12){
                                int bitBloqueA = buscarBit(stream,'B',fit);//Apuntador
                                //Guardamos el bloque en el inodo archivo
                                fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*numInodo,SEEK_SET);
                                fread(&inodo,sizeof(InodoTable),1,stream);
                                inodo.i_block[i] = bitBloqueA;
                                fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*numInodo,SEEK_SET);
                                fwrite(&inodo,sizeof(InodoTable),1,stream);
                                //Registramos el bloque apuntador en el bitmap
                                fseek(stream,super.s_bm_block_start + bitBloqueA,SEEK_SET);
                                fwrite(&buffer3,sizeof(char),1,stream);
                                //Creamos el bloque de apuntadores como es simple solo se debe crear uno
                                int bitBloque = buscarBit(stream,'B',fit);//Primer bloque archivo
                                apuntadores.b_pointer[0] = bitBloque;
                                for(int i = 1; i < 16; i++)
                                    apuntadores.b_pointer[i] = -1;
                                fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueApuntadores))*bitBloqueA,SEEK_SET);
                                fwrite(&apuntadores,sizeof(BloqueApuntadores),1,stream);
                                //Empezamos con la creacion de bloques archivos
                                //Registramos el primer bloque en el bitmap
                                fseek(stream,super.s_bm_block_start + bitBloque,SEEK_SET);
                                fwrite(&buffer2,sizeof(char),1,stream);
                                if(caracteres > 63){
                                    for(int j = 0; j < 63; j++){
                                        if(content.length() != 0){//-cont
                                            archivo.b_content[j] = content[contChar];
                                            contChar++;
                                        }else{//-size
                                            archivo.b_content[j] = contentSize[charNum];
                                            charNum++;
                                            if(charNum == 10)
                                                charNum = 0;
                                        }
                                    }
                                    //Guardamos el bloque
                                    fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueArchivo))*bitBloque,SEEK_SET);
                                    fwrite(&archivo,sizeof(BloqueArchivo),1,stream);
                                    caracteres -= 63;
                                }else{
                                    for (int j = 0; j < caracteres; j++) {
                                        if(content.length() != 0){
                                            archivo.b_content[j] = content[contChar];
                                            contChar++;
                                        }else{
                                            archivo.b_content[j] = contentSize[charNum];
                                            charNum++;
                                            if(charNum == 10)
                                                charNum = 0;
                                        }
                                    }
                                    //Guardamos el bloque
                                    fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueArchivo))*bitBloque,SEEK_SET);
                                    fwrite(&archivo,sizeof(BloqueArchivo),1,stream);
                                }
                            }else if(i > 12 && i < 28){
                                int libre = 0;
                                fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*numInodo,SEEK_SET);
                                fread(&inodo,sizeof(InodoTable),1,stream);
                                fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueApuntadores))*inodo.i_block[12],SEEK_SET);
                                fread(&apuntadores,sizeof(BloqueApuntadores),1,stream);
                                for (int a = 0; a < 16; a++) {
                                    if(apuntadores.b_pointer[a] == -1){
                                        libre = a;
                                        break;
                                    }
                                }
                                int bitBloque = buscarBit(stream,'B',fit);
                                apuntadores.b_pointer[libre] = bitBloque;
                                fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueApuntadores))*inodo.i_block[12],SEEK_SET);
                                fwrite(&apuntadores,sizeof(BloqueApuntadores),1,stream);
                                //Registramos el bloque en el bitmap
                                fseek(stream,super.s_bm_block_start + bitBloque,SEEK_SET);
                                fwrite(&buffer2,sizeof(char),1,stream);
                                if(caracteres > 63){
                                    for(int j = 0; j < 63; j++){
                                        if(content.length() != 0){//-cont
                                            archivo.b_content[j] = content[contChar];
                                            contChar++;
                                        }else{//-size
                                            archivo.b_content[j] = contentSize[charNum];
                                            charNum++;
                                            if(charNum == 10)
                                                charNum = 0;
                                        }
                                    }
                                    //Guardamos el bloque
                                    fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueArchivo))*bitBloque,SEEK_SET);
                                    fwrite(&archivo,sizeof(BloqueArchivo),1,stream);
                                    caracteres -= 63;
                                }else{
                                    for (int j = 0; j < caracteres; j++) {
                                        if(content.length() != 0){
                                            archivo.b_content[j] = content[contChar];
                                            contChar++;
                                        }else{
                                            archivo.b_content[j] = contentSize[charNum];
                                            charNum++;
                                            if(charNum == 10)
                                                charNum = 0;
                                        }
                                    }
                                    //Guardamos el bloque
                                    fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueArchivo))*bitBloque,SEEK_SET);
                                    fwrite(&archivo,sizeof(BloqueArchivo),1,stream);
                                }

                            }else if(i == 29){//Apuntador doble

                            }else{//Apuntadores simples
                                int bitBloque = buscarBit(stream,'B',fit);
                                //Registramos el bloque en el bitmap
                                fseek(stream,super.s_bm_block_start + bitBloque,SEEK_SET);
                                fwrite(&buffer2,sizeof(char),1,stream);
                                if(caracteres > 63){
                                    for(int j = 0; j < 63; j++){
                                        if(content.length() != 0){//-cont
                                            archivo.b_content[j] = content[contChar];
                                            contChar++;
                                        }else{//-size
                                            archivo.b_content[j] = contentSize[charNum];
                                            charNum++;
                                            if(charNum == 10)
                                                charNum = 0;
                                        }
                                    }
                                    //Guardamos el bloque en el respectivo inodo archivo
                                    fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*numInodo,SEEK_SET);
                                    fread(&inodo,sizeof(InodoTable),1,stream);
                                    inodo.i_block[i] = bitBloque;
                                    fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*numInodo,SEEK_SET);
                                    fwrite(&inodo,sizeof(InodoTable),1,stream);
                                    //Guardamos el bloque
                                    fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueArchivo))*bitBloque,SEEK_SET);
                                    fwrite(&archivo,sizeof(BloqueArchivo),1,stream);
                                    caracteres -= 63;
                                }else{
                                    for (int j = 0; j < caracteres; j++) {
                                        if(content.length() != 0){
                                            archivo.b_content[j] = content[contChar];
                                            contChar++;
                                        }else{
                                            archivo.b_content[j] = contentSize[charNum];
                                            charNum++;
                                            if(charNum == 10)
                                                charNum = 0;
                                        }
                                    }
                                    //Guardamos el bloque en el respectivo inodo archivo
                                    fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*numInodo,SEEK_SET);
                                    fread(&inodo,sizeof(InodoTable),1,stream);
                                    inodo.i_block[i] = bitBloque;
                                    fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*numInodo,SEEK_SET);
                                    fwrite(&inodo,sizeof(InodoTable),1,stream);
                                    //Guardamos el bloque
                                    fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueArchivo))*bitBloque,SEEK_SET);
                                    fwrite(&archivo,sizeof(BloqueArchivo),1,stream);
                                }
                            }
                        }
                        //Modificamos el super bloque
                        super.s_free_blocks_count = super.s_free_blocks_count - numBloques;
                        super.s_free_inodes_count = super.s_free_inodes_count - 1;
                        super.s_first_ino = super.s_first_ino + 1;
                        super.s_first_blo = super.s_first_blo + numBloques;
                        fseek(stream,daLoguer.inicioSuper,SEEK_SET);
                        fwrite(&super,sizeof(SuperBloque),1,stream);
                        return 1;
                    }
                    super.s_free_inodes_count = super.s_free_inodes_count - 1;
                    super.s_first_ino = super.s_first_ino + 1;
                    fseek(stream,daLoguer.inicioSuper,SEEK_SET);
                    fwrite(&super,sizeof(SuperBloque),1,stream);
                    return 1;
                }else
                    return 2;
            }
        }
    }else{//Directorio
        int existe = buscarCarpetaArchivo(stream,directorio);
        if(existe == -1){
            if(flagP){
                int index = 0;
                string aux = "";
                //Crear posibles carpetas inexistentes
                for (int i = 0; i < cont; i++) {
                    if(i == cont -1){
                        char dir[100] = "/";
                        strcat(dir,nombreCarpeta);
                        return nuevoArchivo(stream,fit,false,dir,size,contenido,index,auxPath);
                    }else{
                        aux += "/"+lista.at(i);
                        char dir[500];
                        char auxDir[500];
                        strcpy(dir,aux.c_str());
                        strcpy(auxDir,aux.c_str());
                        int carpeta = buscarCarpetaArchivo(stream,dir);
                        if(carpeta == -1){
                            nuevaCarpeta(stream,fit,false,auxDir,index);
                            strcpy(auxDir,aux.c_str());
                            index = buscarCarpetaArchivo(stream,auxDir);
                        }else
                            index = carpeta;
                    }
                }
            }else
                return 4;
        }else{//Crear el archivo en el directorio
            char dir[100] = "/";
            strcat(dir,nombreCarpeta);
            return nuevoArchivo(stream,fit,false,dir,size,contenido,existe,auxPath);
        }
    }

    return 0;
}

int MKFILE::buscarBit(FILE *fp, char tipo, char fit){
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

InodoTable MKFILE::crearInodo(int size,char type,int perm){
    InodoTable inodo ;
    inodo.i_uid = daLoguer.id_user;
    inodo.i_gid = daLoguer.id_grp;
    inodo.i_size = size;
    inodo.i_atime = time(nullptr);
    inodo.i_ctime = time(nullptr);
    inodo.i_mtime = time(nullptr);
    for(int i = 0; i < 15; i++)
        inodo.i_block[i] = -1;
    inodo.i_type = type;
    inodo.i_perm = perm;
    return inodo;
}

int MKFILE::nuevaCarpeta(FILE *stream, char fit, bool flagP, char *path, int index){
    SuperBloque super;
    InodoTable inodo,inodoNuevo;
    BloqueCarpeta carpeta, carpetaNueva, carpetaAux;
    BloqueApuntadores apuntadores;
    QList<string> lista = QList<string>();
    char copiaPath[500];
    char directorio[500];
    char nombreCarpeta[80];

    strcpy(copiaPath,path);
    strcpy(directorio,dirname(copiaPath));
    strcpy(copiaPath,path);
    strcpy(nombreCarpeta,basename(copiaPath));
    char *token = strtok(path,"/");
    int cont = 0;
    int numInodo = index;
    int response = 0;

    while(token != nullptr){
        cont++;
        lista.append(token);
        token = strtok(nullptr,"/");
    }

    fseek(stream,daLoguer.inicioSuper,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,stream);

    if(cont == 1){//Solo es una carpeta '/home' | '/archivos'
        int content = 0;
        int bloque = 0;
        int pointer = 0;
        int libre = buscarContentLibre(stream,numInodo,inodo,carpeta,apuntadores,content,bloque,pointer);
        if(libre == 1){
            if(bloque == 12){//Apuntador indirecto simple
                bool permissions = permisosDeEscritura(inodo.i_perm,(inodo.i_uid == daLoguer.id_user),(inodo.i_gid == daLoguer.id_grp));
                if(permissions || (daLoguer.id_user == 1 && daLoguer.id_grp == 1) ){
                    char buffer = '1';
                    int bitInodo = buscarBit(stream,'I',fit);
                    //Agregamos la carpeta al espacio libre en el bloque
                    carpeta.b_content[content].b_inodo = bitInodo;
                    strcpy(carpeta.b_content[content].b_name,nombreCarpeta);
                    fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*apuntadores.b_pointer[pointer],SEEK_SET);
                    fwrite(&carpeta,sizeof(BloqueCarpeta),1,stream);
                    //Creamos el nuevo inodo carpeta
                    inodoNuevo = crearInodo(0,'0',664);
                    int bitBloque = buscarBit(stream,'B',fit);
                    inodoNuevo.i_block[0] = bitBloque;
                    fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*bitInodo,SEEK_SET);
                    fwrite(&inodoNuevo,sizeof(InodoTable),1,stream);
                    //Guardamos el bit del inodo en el bitmap
                    fseek(stream,super.s_bm_inode_start + bitInodo,SEEK_SET);
                    fwrite(&buffer,sizeof(char),1,stream);
                    //Creamos el nuevo bloque carpeta
                    carpetaNueva = crearBloqueCarpeta();
                    carpetaNueva.b_content[0].b_inodo = bitInodo;
                    carpetaNueva.b_content[1].b_inodo = numInodo;
                    strcpy(carpetaNueva.b_content[0].b_name,".");
                    strcpy(carpetaNueva.b_content[1].b_name,"..");
                    fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*bitBloque,SEEK_SET);
                    fwrite(&carpetaNueva,sizeof(BloqueCarpeta),1,stream);
                    //Guardamos el bit del bloque en el bitmap
                    fseek(stream,super.s_bm_block_start + bitBloque,SEEK_SET);
                    fwrite(&buffer,sizeof(char),1,stream);
                    //Sobreescribimos el super bloque
                    super.s_free_inodes_count = super.s_free_inodes_count - 1;
                    super.s_free_blocks_count = super.s_free_blocks_count - 1;
                    super.s_first_ino = super.s_first_ino + 1;
                    super.s_first_blo = super.s_first_blo + 1;
                    fseek(stream,daLoguer.inicioSuper,SEEK_SET);
                    fwrite(&super,sizeof(SuperBloque),1,stream);
                    return 1;
                }else
                    return 2;
            }else if(bloque == 13){//Apuntador indirecto doble

            }else if(bloque == 14){//Apuntador indirecto triple

            }else{//Apuntadores directos
                bool permisos = permisosDeEscritura(inodo.i_perm,(inodo.i_uid == daLoguer.id_user),(inodo.i_gid == daLoguer.id_grp));
                if(permisos || (daLoguer.id_user == 1 && daLoguer.id_grp == 1) ){
                    char buffer = '1';
                    int bitInodo = buscarBit(stream,'I',fit);
                    //Agregamos la carpeta al espacio libre en el bloque
                    carpeta.b_content[content].b_inodo = bitInodo;
                    strcpy(carpeta.b_content[content].b_name,nombreCarpeta);
                    fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*inodo.i_block[bloque],SEEK_SET);
                    fwrite(&carpeta,sizeof(BloqueCarpeta),1,stream);
                    //Creamos el nuevo inodo
                    inodoNuevo = crearInodo(0,'0',664);
                    int bitBloque = buscarBit(stream,'B',fit);
                    inodoNuevo.i_block[0] = bitBloque;
                    fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*bitInodo,SEEK_SET);
                    fwrite(&inodoNuevo,sizeof(InodoTable),1,stream);
                    //Creamos el nuevo bloque carpeta
                    carpetaNueva = crearBloqueCarpeta();
                    carpetaNueva.b_content[0].b_inodo = bitInodo;
                    carpetaNueva.b_content[1].b_inodo = numInodo;
                    strcpy(carpetaNueva.b_content[0].b_name,".");
                    strcpy(carpetaNueva.b_content[1].b_name,"..");
                    fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*bitBloque,SEEK_SET);
                    fwrite(&carpetaNueva,sizeof(BloqueCarpeta),1,stream);
                    //Guardamos los bits en los bitmaps
                    fseek(stream,super.s_bm_inode_start + bitInodo,SEEK_SET);
                    fwrite(&buffer,sizeof(char),1,stream);
                    fseek(stream,super.s_bm_block_start + bitBloque,SEEK_SET);
                    fwrite(&buffer,sizeof(char),1,stream);
                    //Sobreescribimos el super bloque
                    super.s_free_inodes_count = super.s_free_inodes_count - 1;
                    super.s_free_blocks_count = super.s_free_blocks_count - 1;
                    super.s_first_ino = super.s_first_ino + 1;
                    super.s_first_blo = super.s_first_blo + 1;
                    fseek(stream,daLoguer.inicioSuper,SEEK_SET);
                    fwrite(&super,sizeof(SuperBloque),1,stream);
                    return 1;
                }else
                    return 2;
            }
        }else if(libre == 0){//Todos bloques estan llenos
            bool flag = false;//Primera vez
            pointer = -1;
            fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*numInodo,SEEK_SET);
            fread(&inodo,sizeof(InodoTable),1,stream);
            for (int i = 0; i < 15; i++) {
                if(i == 12){//Apuntador indirecto simple
                    if(inodo.i_block[i] == -1){//Primera vez
                        bloque = 12;
                        flag = true;
                        break;
                    }else{
                        fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueApuntadores))*inodo.i_block[12],SEEK_SET);
                        fread(&apuntadores,sizeof(BloqueApuntadores),1,stream);
                        for(int j = 0; j < 16; j++){
                            if(apuntadores.b_pointer[j] == -1){
                                bloque = 12;
                                pointer = j;
                                break;
                            }
                        }
                    }
                    if(flag || pointer!= -1)
                        break;
                }else if(i == 13){

                }else if(i == 14){

                }else{
                    if(inodo.i_block[i] == -1){
                        bloque = i;
                        break;
                    }
                }
            }

            if(bloque == 12 && flag){//Apuntador indirecto simple
                bool permissions = permisosDeEscritura(inodo.i_perm,(inodo.i_uid == daLoguer.id_user),(inodo.i_gid == daLoguer.id_grp));
                if(permissions || (daLoguer.id_user == 1 && daLoguer.id_grp == 1) ){
                    char buffer = '1';
                    char buffer3 = '3';
                    //Guardamos el bloque en el inodo
                    int bitBloque = buscarBit(stream,'B',fit);//Apuntador
                    inodo.i_block[bloque] = bitBloque;
                    fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*numInodo,SEEK_SET);
                    fwrite(&inodo,sizeof(InodoTable),1,stream);
                    //Escribimos el bit del bloque apuntador en el bitmap
                    fseek(stream,super.s_bm_block_start + bitBloque,SEEK_SET);
                    fwrite(&buffer3,sizeof(char),1,stream);
                    //Creamos el bloque de apuntadores como es simple solo se debe crear uno
                    int bitBloqueCarpeta = buscarBit(stream,'B',fit);//Carpeta
                    apuntadores.b_pointer[0] = bitBloqueCarpeta;
                    for(int i = 1; i < 16; i++)
                        apuntadores.b_pointer[i] = -1;
                    fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueApuntadores))*bitBloque,SEEK_SET);
                    fwrite(&apuntadores,sizeof(BloqueApuntadores),1,stream);
                    //Creamos la carpeta del apuntador
                    int bitInodo = buscarBit(stream,'I',fit);
                    carpetaAux = crearBloqueCarpeta();
                    carpetaAux.b_content[0].b_inodo = bitInodo;
                    strcpy(carpetaAux.b_content[0].b_name,nombreCarpeta);
                    fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*bitBloqueCarpeta,SEEK_SET);
                    fwrite(&carpetaAux,sizeof(BloqueCarpeta),1,stream);
                    //Escribimos el bit del bloque carpeta en el bitmap
                    fseek(stream,super.s_bm_block_start + bitBloqueCarpeta,SEEK_SET);
                    fwrite(&buffer,sizeof(char),1,stream);
                    //Creamos el nuevo inodo carpeta
                    inodoNuevo = crearInodo(0,'0',664);
                    bitBloque = buscarBit(stream,'B',fit);//Carpeta
                    inodoNuevo.i_block[0] = bitBloque;
                    fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*bitInodo,SEEK_SET);
                    fwrite(&inodoNuevo,sizeof(InodoTable),1,stream);
                    //Escribimos el bit del inodo en el bitmap
                    fseek(stream,super.s_bm_inode_start + bitInodo,SEEK_SET);
                    fwrite(&buffer,sizeof(char),1,stream);
                    //Creamos el nuevo bloque carpeta
                    carpetaNueva = crearBloqueCarpeta();
                    carpetaNueva.b_content[0].b_inodo = bitInodo;
                    carpetaNueva.b_content[1].b_inodo = numInodo;
                    strcpy(carpetaNueva.b_content[0].b_name,".");
                    strcpy(carpetaNueva.b_content[1].b_name,"..");
                    fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*bitBloque,SEEK_SET);
                    fwrite(&carpetaNueva,sizeof(BloqueCarpeta),1,stream);
                    //Guardamos el bit en el bitmap de bloques
                    fseek(stream,super.s_bm_block_start + bitBloque,SEEK_SET);
                    fwrite(&buffer,sizeof(char),1,stream);
                    //Sobreescribimos el super bloque
                    super.s_free_inodes_count = super.s_free_inodes_count - 1;
                    super.s_free_blocks_count = super.s_free_blocks_count - 3;
                    super.s_first_ino = super.s_first_ino + 1;
                    super.s_first_blo = super.s_first_blo + 3;
                    fseek(stream,daLoguer.inicioSuper,SEEK_SET);
                    fwrite(&super,sizeof(SuperBloque),1,stream);
                    return 1;
                }else
                    return 2;
            }else if(bloque == 12 && !flag){
                char buffer = '1';
                //Escribir el numero de bloque en el bloque de apuntadores
                int bitBloque = buscarBit(stream,'B',fit);
                apuntadores.b_pointer[pointer] = bitBloque;
                fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueApuntadores))*inodo.i_block[12],SEEK_SET);
                fwrite(&apuntadores,sizeof(BloqueApuntadores),1,stream);
                //Creamos el bloque auxiliar
                int bitInodo = buscarBit(stream,'I',fit);
                carpetaAux = crearBloqueCarpeta();
                carpetaAux.b_content[0].b_inodo = bitInodo;
                strcpy(carpetaAux.b_content[0].b_name,nombreCarpeta);
                fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*bitBloque,SEEK_SET);
                fwrite(&carpetaAux,sizeof(BloqueCarpeta),1,stream);
                //Escribimos el bit del bloque carpeta en el bitmap
                fseek(stream,super.s_bm_block_start + bitBloque,SEEK_SET);
                fwrite(&buffer,sizeof(char),1,stream);
                //Creamos el nuevo inodo carpeta
                inodoNuevo = crearInodo(0,'0',664);
                inodoNuevo.i_uid = daLoguer.id_user;
                inodoNuevo.i_gid = daLoguer.id_grp;
                bitBloque = buscarBit(stream,'B',fit);//Carpeta
                inodoNuevo.i_block[0] = bitBloque;
                fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*bitInodo,SEEK_SET);
                fwrite(&inodoNuevo,sizeof(InodoTable),1,stream);
                //Escribimos el bit del inodo en el bitmap
                fseek(stream,super.s_bm_inode_start + bitInodo,SEEK_SET);
                fwrite(&buffer,sizeof(char),1,stream);
                //Creamos el nuevo bloque carpeta
                carpetaNueva = crearBloqueCarpeta();
                carpetaNueva.b_content[0].b_inodo = bitInodo;
                carpetaNueva.b_content[1].b_inodo = numInodo;
                strcpy(carpetaNueva.b_content[0].b_name,".");
                strcpy(carpetaNueva.b_content[1].b_name,"..");
                fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*bitBloque,SEEK_SET);
                fwrite(&carpetaNueva,sizeof(BloqueCarpeta),1,stream);
                //Guardamos el bit en el bitmap de bloques
                fseek(stream,super.s_bm_block_start + bitBloque,SEEK_SET);
                fwrite(&buffer,sizeof(char),1,stream);
                //Sobreescribimos el super bloque
                super.s_free_inodes_count = super.s_free_inodes_count - 1;
                super.s_free_blocks_count = super.s_free_blocks_count - 2;
                super.s_first_ino = super.s_first_ino + 1;
                super.s_first_blo = super.s_first_blo + 2;
                fseek(stream,daLoguer.inicioSuper,SEEK_SET);
                fwrite(&super,sizeof(SuperBloque),1,stream);
                return 1;
            }
            else if(bloque == 13){//Apuntador indirecto doble

            }else if(bloque == 14){//Apuntador indirecto triple

            }else{//Apuntadores directos
                bool permissions = permisosDeEscritura(inodo.i_perm,(inodo.i_uid == daLoguer.id_user),(inodo.i_gid == daLoguer.id_grp));
                if(permissions || (daLoguer.id_user == 1 && daLoguer.id_grp == 1) ){
                    char buffer = '1';
                    int bitBloque = buscarBit(stream,'B',fit);
                    inodo.i_block[bloque] = bitBloque;
                    //Sobreescribimos el inodo
                    fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*numInodo,SEEK_SET);
                    fwrite(&inodo,sizeof(InodoTable),1,stream);
                    //Bloque carpeta auxiliar
                    int bitInodo = buscarBit(stream,'I',fit);
                    carpetaAux = crearBloqueCarpeta();
                    carpetaAux.b_content[0].b_inodo = bitInodo;
                    strcpy(carpetaAux.b_content[0].b_name,nombreCarpeta);
                    fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*bitBloque,SEEK_SET);
                    fwrite(&carpetaAux,sizeof(BloqueCarpeta),1,stream);
                    //Escribimos el bit en el bitmap de blqoues
                    fseek(stream,super.s_bm_block_start + bitBloque,SEEK_SET);
                    fwrite(&buffer,sizeof(char),1,stream);
                    //Creamos el nuevo inodo
                    inodoNuevo = crearInodo(0,'0',664);
                    bitBloque = buscarBit(stream,'B',fit);
                    inodoNuevo.i_block[0] = bitBloque;
                    fseek(stream,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*bitInodo,SEEK_SET);
                    fwrite(&inodoNuevo,sizeof(InodoTable),1,stream);
                    //Escribimos el bit en el bitmap de inodos
                    fseek(stream,super.s_bm_inode_start + bitInodo,SEEK_SET);
                    fwrite(&buffer,sizeof(char),1,stream);
                    //Creamos el nuevo bloque carpeta
                    carpetaNueva = crearBloqueCarpeta();
                    carpetaNueva.b_content[0].b_inodo = bitInodo;
                    carpetaNueva.b_content[1].b_inodo = numInodo;
                    strcpy(carpetaNueva.b_content[0].b_name,".");
                    strcpy(carpetaNueva.b_content[1].b_name,"..");
                    fseek(stream,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*bitBloque,SEEK_SET);
                    fwrite(&carpetaNueva,sizeof(BloqueCarpeta),1,stream);
                    //Guardamos el bit en el bitmap de bloques
                    fseek(stream,super.s_bm_block_start + bitBloque,SEEK_SET);
                    fwrite(&buffer,sizeof(char),1,stream);
                    //Sobreescribimos el super bloque
                    super.s_free_inodes_count = super.s_free_inodes_count - 1;
                    super.s_free_blocks_count = super.s_free_blocks_count - 2;
                    super.s_first_ino = super.s_first_ino + 1;
                    super.s_first_blo = super.s_first_blo + 2;
                    fseek(stream,daLoguer.inicioSuper,SEEK_SET);
                    fwrite(&super,sizeof(SuperBloque),1,stream);
                    return 1;
                }else
                    return 2;
            }

        }
    }else{//Es un directorio '/home/usac/archivos'
        //Verificar que exista el directorio
        int existe = buscarCarpetaArchivo(stream,directorio);
        if(existe == -1){
            if(flagP){
                int index = 0;
                string aux = "";
                //Crear posibles carpetas inexistentes
                for(int i = 0; i < cont; i++){
                    aux += "/"+lista.at(i);
                    char dir[500];
                    char auxDir[500];
                    strcpy(dir,aux.c_str());
                    strcpy(auxDir,aux.c_str());
                    int carpeta = buscarCarpetaArchivo(stream,dir);
                    if(carpeta == -1){
                        response = nuevaCarpeta(stream,fit,false,auxDir,index);
                        if(response == 2)
                            break;
                        strcpy(auxDir,aux.c_str());
                        index = buscarCarpetaArchivo(stream,auxDir);
                    }else
                        index = carpeta;
                }
            }else
                return 3;
        }else{//Solo crear la carpeta en el directorio
            char dir[100] = "/";
            strcat(dir,nombreCarpeta);
            return nuevaCarpeta(stream,fit,false,dir,existe);
        }
    }

    return response;
}

BloqueCarpeta MKFILE::crearBloqueCarpeta(){
    BloqueCarpeta carpeta;

    for(int i = 0; i < 4; i++){
        strcpy(carpeta.b_content[i].b_name,"");
        carpeta.b_content[i].b_inodo = -1;
    }

    return carpeta;
}
