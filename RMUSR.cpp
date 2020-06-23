#include "RMUSR.h"

RMUSR::RMUSR(string user)
{
    this->user = user;
}

void RMUSR::Ejecutar(){
    if(login){
        if(daLoguer.id_user == 1 && daLoguer.id_grp == 1){
            if(BuscarUsuario(this->user)){
                EliminarUsuario(this->user);
            }else{
                cout << "No existe el usuario" << endl;
            }
        }else{
            cout << "Debe ser usuario root para usar el comando" << endl;
        }
    }else{
        cout << "Debe iniciar sesion para usar el comando" << endl;
    }
}

bool RMUSR::BuscarUsuario(string name){
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


void RMUSR::EliminarUsuario(string name){
    FILE *fp = fopen(daLoguer.direccion.c_str(),"r+b");

        SuperBloque super;
        InodoTable inodo;
        BloqueArchivo archivo;

        int col = 1;
        char actual;
        string palabra = "";
        int posicion = 0;
        int numBloque = 0;
        int id = -1;
        char tipo = '\0';
        string grupo = "";
        string usuario = "";
        bool flag = false;

        fseek(fp,daLoguer.inicioSuper,SEEK_SET);
        fread(&super,sizeof(SuperBloque),1,fp);
        //Nos posicionamos en el inodo del archivo users.txt
        fseek(fp,super.s_inode_start + static_cast<int>(sizeof(InodoTable)),SEEK_SET);
        fread(&inodo,sizeof(InodoTable),1,fp);

        for (int i = 0; i < 12; i++) {
            if(inodo.i_block[i] != -1){
                fseek(fp,super.s_block_start + static_cast<int>(sizeof(BloqueArchivo))*inodo.i_block[i],SEEK_SET);
                fread(&archivo,sizeof(BloqueArchivo),1,fp);
                for(int j = 0; j < 63; j++){
                    actual = archivo.b_content[j];
                    if(actual=='\n'){
                        if(tipo == 'U'){
                            if(strcmp(usuario.c_str(),name.c_str()) == 0){
                                fseek(fp,super.s_block_start+static_cast<int>(sizeof(BloqueArchivo))*numBloque,SEEK_SET);
                                fread(&archivo,sizeof(BloqueArchivo),1,fp);
                                archivo.b_content[posicion] = '0';
                                fseek(fp,super.s_block_start+static_cast<int>(sizeof(BloqueArchivo))*numBloque,SEEK_SET);
                                fwrite(&archivo,sizeof(BloqueArchivo),1,fp);
                                cout << "Usuario eliminado con exito" << endl;
                                char aux[500];
                                char operacion[8];
                                string datos = "Usuario: "+this->user;
                                strcpy(aux,datos.c_str());
                                strcpy(operacion,"RMUSR");
                                guardarJournal(operacion,1,664,aux);
                                flag = true;
                                break;
                            }
                            usuario = "";
                            grupo = "";
                        }
                        col = 1;
                        palabra = "";
                    }else if(actual != ','){
                        palabra += actual;
                        col++;
                    }else if(actual == ','){
                        if(col == 2){
                            id = atoi(palabra.c_str());
                            posicion = j-1;
                            numBloque = inodo.i_block[i];
                        }
                        else if(col == 4)
                            tipo = palabra[0];
                        else if(grupo == "")
                            grupo = palabra;
                        else if(usuario == "")
                            usuario = palabra;
                        col++;
                        palabra = "";
                    }
                }
                if(flag)
                    break;
            }
        }

        fclose(fp);
}

void RMUSR::guardarJournal(char *operacion, int tipo, int permisos, char *nombre){
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
