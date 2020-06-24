#include "FIND.h"

FIND::FIND(string path, string name)
{
    this->path = path;
    this->name = name;
}


void FIND::Ejecutar(){
    if(login){
        char auxPath[500];
        strcpy(auxPath,path.c_str());
        FILE *fp = fopen(daLoguer.direccion.c_str(),"r+b");
        int carpetaInicio;
        if(path!="/")
            carpetaInicio = buscarCarpetaArchivo(fp,auxPath);
        else
            carpetaInicio = 0;
        if(carpetaInicio != -1){
            QList<string> lista = QList<string>();
            Files busqueda = buscarRuta(this->name);
            char rutaAux[500];
            memset(rutaAux,'\0',sizeof (rutaAux));
            strcpy(rutaAux, busqueda.path.c_str());
            char *token = strtok(rutaAux,"/");
            int cont = 0;
            while(token != nullptr){
                lista.append(token);
                cont++;
                token = strtok(nullptr,"/");
            }
            string route = "";
            getRuta(fp, route, 0, carpetaInicio);
            if(route.find(name) != string::npos){
                cout << route << endl;
            }else{
                cout << "No se encontro el archivo" << endl;
            }
        }else{
            cout << "No se encontro la carpeta donde desea comenzar la busqueda" << endl;
        }
        fclose(fp);
    }else{
        cout << "Debe iniciar sesion para utilizar el comando" << endl;
    }
}

void FIND::getRuta(FILE *file, string &aux, int tabs, int inodePos){
    SuperBloque sb;
    fseek(file, daLoguer.inicioSuper, SEEK_SET);
    fread(&sb, sizeof (SuperBloque), 1, file);

    Inodo in;
    int inodeSize = static_cast<int>(sizeof (Inodo));
    fseek(file, sb.s_inode_start + inodeSize*inodePos, SEEK_SET);
    fread(&in, sizeof (Inodo), 1, file);

    for(int i=0; i < 15; i++){
        if(in.i_block[i] == -1)
            continue;

        if(i < 12){
            if(in.i_type == '0'){
                BloqueCarpeta fb;

                int folderSize = static_cast<int>(sizeof (BloqueCarpeta));
                fseek(file, sb.s_block_start + folderSize*in.i_block[i], SEEK_SET);
                fread(&fb, sizeof (BloqueCarpeta), 1, file);

                string sTabs = "";
                for(int i=0; i < tabs; i++)
                    sTabs += "  ";
                string auxS = "";
                for(int i = 0; i < 4; i++){
                    auxS = fb.b_content[i].b_name;
                    if((auxS.length() > 1 && auxS[0] != '.') || (auxS.length() > 1 && auxS[1] != '.')){

                        aux += sTabs + "| " + auxS + "\n";

                        tabs++;
                        getRuta(file, aux, tabs, fb.b_content[i].b_inodo);
                        tabs--;
                    }
                }
            }
        }else if(i = 12){

        }
    }

}

Files FIND::buscarRuta(string name){
    for(int i = 0; i < files->size(); i++){
        QFileInfo fileName(QString::fromStdString(files->at(i).path));
        string nombre = fileName.fileName().toStdString();
        if(nombre == name){
            return files->at(i);
        }
    }
    Files file;
    file.path = "";
    return file;
}

void FIND::guardarJournal(char *operacion, int tipo, int permisos, char *nombre){
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

int FIND::buscarCarpetaArchivo(FILE *stream, char* path){
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

int FIND::byteInodoBloque(FILE *stream,int pos, char tipo){
    SuperBloque super;
    fseek(stream,daLoguer.inicioSuper,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,stream);
    if(tipo == '1'){
        return (super.s_inode_start + static_cast<int>(sizeof(InodoTable))*pos);
    }else if(tipo == '2')
        return (super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*pos);
    return 0;
}
