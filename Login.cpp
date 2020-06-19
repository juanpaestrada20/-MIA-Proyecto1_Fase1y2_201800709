#include "Login.h"

Login::Login(string user, string pass, string id)
{
    this->user = user;
    this->pass = pass;
    this->id = id;
    this->ruta = "";
    this->name = "";
    this->montaje = *new Mount("", "", "", 0);
}

void Login::Ejecutar(){
    if(login){
        cout << "Ya hay una sesion actva" << endl;
        return;
    }
    if(GetRuta(this->id)){
        int index = -1;
        FILE* disco = fopen(this->ruta.c_str(), "r+b");
        if(disco == NULL){
            cout << "No se pudo abrir el disco, intentelo de nuevo " << endl;
            return;
        }
        MBR masterboot;
        SuperBloque super;
        InodoTable inodo;
        fread(&masterboot,sizeof(MBR),1,disco);
        for(int i = 0; i < masterboot.partitions->size; i++){
            if(masterboot.partitions[i].status =='1'){
                if(strcmp(masterboot.partitions[i].name, this->montaje.name.c_str()) == 0){
                    index = i;
                    break;
                }
            }
        }
        fseek(disco,masterboot.partitions[index].start,SEEK_SET);
        fread(&super,sizeof(SuperBloque),1,disco);
        fseek(disco,super.s_inode_start + sizeof(Inodo),SEEK_SET);
        fread(&inodo,sizeof(InodoTable),1,disco);
        fseek(disco,super.s_inode_start + sizeof(Inodo),SEEK_SET);
        inodo.i_atime = time(nullptr);
        fwrite(&inodo,sizeof(InodoTable),1,disco);
        fclose(disco);
        daLoguer.inicioSuper = masterboot.partitions[index].start;
        daLoguer.fit = masterboot.partitions[index].fit;
        daLoguer.inicioJournal = masterboot.partitions[index].start + sizeof(SuperBloque);
        daLoguer.tipo_sistema = super.s_filesystem_type;
        bool correcto = ComprobarLogin(this->user, this->pass, this->ruta);
        if(correcto){
            login = true;
            cout << "Sesion iniciada con exito" << endl;
        }else{
            cout << "Usuario y/o Contraseña incorrecta" << endl;
        }

    }else{
        cout << "Debe montar la particion con el id: " << this->id << endl;
    }
}

bool Login::GetRuta(string id){
    QList<Mount>::iterator i;
    int pos = 0;
    for(i = montajes->begin(); i!=montajes->end(); i++ )
    {
        if(i->id == id)
        {
            this->ruta = i->path;
            this->montaje = montajes->at(pos);
            return true;
        }
        pos++;
    }
    return false;
}

bool Login::ComprobarLogin(string user, string password, string direccion){
    FILE *fp = fopen(direccion.c_str(),"rb+");

        char cadena[400] = "\0";
        SuperBloque super;
        InodoTable inodo;

        fseek(fp,daLoguer.inicioSuper,SEEK_SET);
        fread(&super,sizeof(SuperBloque),1,fp);
        //Leemos el inodo del archivo users.txt
        fseek(fp,super.s_inode_start + static_cast<int>(sizeof(InodoTable)),SEEK_SET);
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
            string group;
            char user_[12];
            char password_[12];
            char *end_token;
            char *token2 = strtok_r(token,",",&end_token);
            strcpy(id,token2);
            if(strcmp(id,"0") != 0){//Verificar que no sea un U/G eliminado
                token2=strtok_r(nullptr,",",&end_token);
                strcpy(tipo,token2);
                if(strcmp(tipo,"U") == 0){
                    token2 = strtok_r(nullptr,",",&end_token);
                    group = token2;
                    token2 = strtok_r(nullptr,",",&end_token);
                    strcpy(user_,token2);
                    token2 = strtok_r(nullptr,",",&end_token);
                    strcpy(password_,token2);
                    if(strcmp(user_,user.c_str()) == 0){
                        if(strcmp(password_,password.c_str()) == 0){
                            daLoguer.direccion = direccion;
                            daLoguer.id_user = atoi(id);
                            daLoguer.id_grp = Get_Id_Group(group);
                            return true;
                        }else
                            return false;
                    }
                }
            }
            token = strtok_r(nullptr,"\n",&end_str);
        }

        return false;
}

void Login::Eliminar_Espacios(char *cadena){
    char *p;
    p =  strchr(cadena,  ' ');
    if(p)
        *p = '\0';
}

int Login::Get_Id_Group(string name)
{
    FILE *fp = fopen(daLoguer.direccion.c_str(),"rb+");

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

