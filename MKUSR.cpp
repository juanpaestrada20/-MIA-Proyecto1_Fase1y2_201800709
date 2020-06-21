#include "MKUSR.h"

MKUSR::MKUSR(string user, string password, string grupo)
{
    this->user = user;
    this->password = password;
    this->grupo = grupo;
}

void MKUSR::Ejecutar(){
    if(login){
        if(daLoguer.id_user == 1 && daLoguer.id_grp == 1){
            if(this->user.length() <= 10 && this->password.length() <= 10 && this->grupo.length() <= 10){
                if(buscarGrupo(this->grupo) != 1){
                    if(!buscarUsuario(this->user)){
                        int id = getIdUser();
                        string datos = to_string(id)+",U,"+this->grupo+","+this->user+","+this->password+"\n";
                        agregarUsuario(datos);
                        cout << "Usuario creado con exito!" << endl;
                    }else{
                        cout << "El usuario ya existe" << endl;
                    }
                }else{
                    cout << "No existe el grupo donde desea crear el usuario" << endl;
                }
            }else{
                cout << "El usuario, contraseña y grupo no deben exceder los 10 caracteres" << endl;
            }
        }else{
            cout << "Deber ser usuario root para usar el comando" << endl;
        }
    }else{
        cout << "Debe iniciar sesion para usar el comando" << endl;
    }
}

int MKUSR::buscarGrupo(string name){
    FILE *fp = fopen(daLoguer.direccion.c_str(),"r+b");

    char cadena[400] = "\0";
    SuperBloque super;
    InodoTable inodo;

    fseek(fp,daLoguer.inicioSuper,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,fp);
    //Leemos el inodo del archivo users.txt
    fseek(fp,super.s_inode_start + static_cast<int>(sizeof(InodoTable)), SEEK_SET);
    fread(&inodo,sizeof(InodoTable),1,fp);

    for(int i = 0; i < 15; i++){
        if(inodo.i_block[i] != -1){
            BloqueArchivo archivo;
            fseek(fp,super.s_block_start,SEEK_SET);
            for(int j = 0; j <= inodo.i_block[i]; j++){
                fread(&archivo,sizeof(BloqueArchivo),1,fp);
            }
            strcat(cadena,archivo.b_content);
        }
    }

    fclose(fp);

    char *end_str;
    char *token = strtok_r(cadena,"\n",&end_str);
    while(token != nullptr){
        char id[2];
        char tipo[2];
        char group[12];
        char *end_token;
        char *token2 = strtok_r(token,",",&end_token);
        strcpy(id,token2);
        if(strcmp(id,"0") != 0){//Verificar que no sea un U/G eliminado
            token2 = strtok_r(nullptr,",",&end_token);
            strcpy(tipo,token2);
            if(strcmp(tipo,"G") == 0){
                strcpy(group,end_token);
                if(strcmp(group,name.c_str()) == 0)
                    return atoi(id);
            }
        }
        token = strtok_r(nullptr,"\n",&end_str);
    }

    return -1;
}

bool MKUSR::buscarUsuario(string name){
    FILE *fp = fopen(daLoguer.direccion.c_str(),"r+b");

    char cadena[400] = "\0";
    SuperBloque super;
    InodoTable inodo;

    fseek(fp,daLoguer.inicioSuper,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,fp);
    //Nos posicionamos en el inodo del archivo users.txt
    fseek(fp,super.s_inode_start + static_cast<int>(sizeof(InodoTable)), SEEK_SET);
    fread(&inodo,sizeof(InodoTable),1,fp);

    for(int i = 0; i < 15; i++){
        if(inodo.i_block[i] != -1){
            BloqueArchivo archivo;
            fseek(fp,super.s_block_start,SEEK_SET);
            for(int j = 0; j <= inodo.i_block[i]; j++){
                fread(&archivo,sizeof(BloqueArchivo),1,fp);
            }
            strcat(cadena,archivo.b_content);
        }
    }

    fclose(fp);

    char *end_str;
    char *token = strtok_r(cadena,"\n",&end_str);
    while(token != nullptr){
        char id[2];
        char tipo[2];
        char user[12];
        char *end_token;
        char *token2 = strtok_r(token,",",&end_token);
        strcpy(id,token2);
        if(strcmp(id,"0") != 0){//Verificar que no sea un U/G eliminado
            token2 = strtok_r(nullptr,",",&end_token);
            strcpy(tipo,token2);
            if(strcmp(tipo,"U") == 0){
                token2 = strtok_r(nullptr,",",&end_token);
                token2 = strtok_r(nullptr,",",&end_token);
                strcpy(user,token2);

                if(strcmp(user,name.c_str()) == 0)
                    return true;
            }
        }
        token = strtok_r(nullptr,"\n",&end_str);
    }

    return false;
}

int MKUSR::getIdUser(){
    FILE *fp = fopen(daLoguer.direccion.c_str(),"r+b");

    char cadena[400] = "\0";
    int res = 0;
    SuperBloque super;
    InodoTable inodo;

    fseek(fp,daLoguer.inicioSuper,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,fp);
    //Nos posicionamos en el inodo del archivo users.txt
    fseek(fp,super.s_inode_start + static_cast<int>(sizeof(InodoTable)), SEEK_SET);
    fread(&inodo,sizeof(InodoTable),1,fp);

    for(int i = 0; i < 15; i++){
        if(inodo.i_block[i] != -1){
            BloqueArchivo archivo;
            fseek(fp,super.s_block_start,SEEK_SET);
            for(int j = 0; j <= inodo.i_block[i]; j++){
                fread(&archivo,sizeof(BloqueArchivo),1,fp);
            }
            strcat(cadena,archivo.b_content);
        }
    }

    fclose(fp);

    char *end_str;
    char *token = strtok_r(cadena,"\n",&end_str);
    while(token != nullptr){
        char id[2];
        char tipo[2];
        char *end_token;
        char *token2 = strtok_r(token,",",&end_token);
        strcpy(id,token2);
        if(strcmp(id,"0") != 0){//Verificar que no sea un U/G eliminado
            token2 = strtok_r(nullptr,",",&end_token);
            strcpy(tipo,token2);
            if(strcmp(tipo,"U") == 0)
                res++;
        }
        token = strtok_r(nullptr,"\n",&end_str);
    }
    return ++res;
}

void MKUSR::agregarUsuario(string datos){
    agregarUsuariosTexto(datos);
    char aux[64];
    char operacion[10];
    strcpy(aux,datos.c_str());
    strcpy(operacion,"mkusr");
    guardarJournal(operacion,1,0,aux);
}

void MKUSR::guardarJournal(char *operacion, int tipo, int permisos, char *nombre){
    SuperBloque super;
    Journal registro;
    memset(registro.journal_name,'\0',sizeof(registro.journal_name));
    memset(registro.journal_operation_type,'\0',sizeof(registro.journal_operation_type));
    strcpy(registro.journal_operation_type,operacion);
    strcpy(registro.journal_name,nombre);
    string comando = "User: " + this->user + ", Pass: "+this->password+", Grupo: "+this->grupo;
    strcpy(registro.operation,comando.c_str());
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

void MKUSR::agregarUsuariosTexto(string d){
    QString datos = QString::fromStdString(d);
    FILE *fp = fopen(daLoguer.direccion.c_str(), "r+b");

    SuperBloque super;
    InodoTable inodo;
    BloqueArchivo archivo;
    int blockIndex = 0;

    fseek(fp,daLoguer.inicioSuper,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,fp);
    //Leemos el inodo del archivo users.txt
    fseek(fp,super.s_inode_start + static_cast<int>(sizeof(InodoTable)), SEEK_SET);
    fread(&inodo,sizeof(InodoTable),1,fp);

    for(int i = 0; i < 12; i++){
        if(inodo.i_block[i] != -1)
            blockIndex = inodo.i_block[i];//Ultimo bloque utilizado del archivo
    }

    fseek(fp,super.s_block_start + static_cast<int>(sizeof(BloqueArchivo))*blockIndex,SEEK_SET);
    fread(&archivo,sizeof(BloqueArchivo),1,fp);
    int enUso = static_cast<int>(strlen(archivo.b_content));
    int libre = 63 - enUso;

    if(datos.length() <= libre){
        strcat(archivo.b_content,datos.toStdString().c_str());
        fseek(fp,super.s_block_start + static_cast<int>(sizeof(BloqueArchivo))*blockIndex,SEEK_SET);
        fwrite(&archivo,sizeof(BloqueArchivo),1,fp);
        fseek(fp,super.s_inode_start + static_cast<int>(sizeof(InodoTable)),SEEK_SET);
        fread(&inodo,sizeof(InodoTable),1,fp);
        inodo.i_size = inodo.i_size + datos.length();
        inodo.i_mtime = time(nullptr);
        fseek(fp,super.s_inode_start + static_cast<int>(sizeof(InodoTable)),SEEK_SET);
        fwrite(&inodo,sizeof(InodoTable),1,fp);
    }else{
        QString aux = "";
        QString aux2 = "";
        int i = 0;

        for(i = 0; i < libre; i++)
            aux += datos.at(i);

        for(; i < datos.length(); i++)
            aux2  += datos.at(i);

        //Guardamos lo que cabe en el primer bloque
        strcat(archivo.b_content,aux.toStdString().c_str());
        fseek(fp,super.s_block_start + static_cast<int>(sizeof(BloqueArchivo))*blockIndex,SEEK_SET);
        fwrite(&archivo,sizeof(BloqueArchivo),1,fp);
        BloqueArchivo auxArchivo;
        strcpy(auxArchivo.b_content,aux2.toStdString().c_str());
        int bit = buscarBit(fp,'B',daLoguer.fit);
        /*Guardamos el bloque en el bitmap y en la tabla de bloques*/
        fseek(fp,super.s_bm_block_start + bit,SEEK_SET);
        fputc('2',fp);
        fseek(fp,super.s_block_start + (static_cast<int>(sizeof(BloqueArchivo))*bit),SEEK_SET);
        fwrite(&auxArchivo,sizeof(BloqueArchivo),1,fp);
        /*Guardamos el modificado del inodo*/
        fseek(fp,super.s_inode_start + static_cast<int>(sizeof(InodoTable)),SEEK_SET);
        fread(&inodo,sizeof(InodoTable),1,fp);
        inodo.i_size = inodo.i_size + datos.length();
        inodo.i_mtime = time(nullptr);
        inodo.i_block[blockIndex] = bit;
        fseek(fp,super.s_inode_start + static_cast<int>(sizeof(InodoTable)),SEEK_SET);
        fwrite(&inodo,sizeof(InodoTable),1,fp);
        /*Guardamos la nueva cantidad de bloques libres y el primer bloque libre*/
        super.s_first_blo = super.s_first_blo + 1;
        super.s_free_blocks_count = super.s_free_blocks_count - 1;
        fseek(fp,daLoguer.inicioSuper,SEEK_SET);
        fwrite(&super,sizeof(SuperBloque),1,fp);
    }
    fclose(fp);
}

int MKUSR::buscarBit(FILE *fp, char tipo, char fit){
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
    if(fit == 'F' || fit == 'f'){//Primer ajuste
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

    }else if(fit == 'B' || fit == 'b'){//Mejor ajuste
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

    }else if(fit == 'W' || fit == 'w'){//Peor ajuste
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
