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

void Login::LeerArchivo(FILE *disco, SuperBloque sb, int size, char* contenido, string ruta){
    int pos = Existe_Archivo(disco,sb,size,ruta);

    if(pos != -1)
    {
        Leer_Inodo_Archivo(disco,sb,size,pos,contenido);
    }
    else
    {
        cout << "No existe el archivo para leer" << endl;
    }
}

int Login::Existe_Archivo(FILE* disco,SuperBloque sblock, int size, string ruta)
{

    return Buscar_Inodo(disco,sblock,size,ruta,0,0);

}

int Login::Buscar_Inodo(FILE *disco, SuperBloque sblock, int size, string ruta,int pos,int cont)
{
    //OBTENER LOS NOMBRES DE LAS RUTAS
    char delim[] = "/";
    char nuevo[1000];
    memset(nuevo,'\0',sizeof(nuevo));
    strcat(nuevo,ruta.c_str());

    char* nombres[1000];
    memset(nombres,'\0',sizeof(nombres));

    char *ptr = strtok(nuevo, delim);

    int aux=0;
    while(ptr != NULL)
    {
        nombres [aux++] = ptr;
        ptr = strtok(NULL, delim);
    }

    //Valores particion
    int r = -1;
    Inodo inodo_aux;
    int n = Calcular_N(size);
    //char Bm_inodo[n];
    char Bm_bloque[3 * n];

    //Inodo
    fseek(disco, Calculo_Posicion_Inodo(sblock,pos) ,SEEK_SET);
    fread(&inodo_aux,sizeof(Inodo),1,disco);

    Modificar_Inodo(disco,inodo_aux,  Calculo_Posicion_Inodo(sblock,pos) ,0);


    //BM de los bloques
    fseek(disco,sblock.s_bm_block_start,SEEK_SET);
    fread(Bm_bloque,sizeof(char),3*n,disco);



    for(int i=0; i < 16 ;i++)
    {
        if(inodo_aux.i_block[i] != -1)
        {
            int pos = inodo_aux.i_block[i];

            if(pos >-1 && pos <16)
            {
                if(Bm_bloque[pos] == '1')        //SI ES UN BLOQUE CARPETA
                {
                    r = Buscar_BCarpeta(disco,sblock,size,ruta,pos,cont);
                    if(r != -1) break;

                }else if(Bm_bloque[pos] == '3')  //SI ES UN BLOQUE APUNTADOR
                {
                    r = Buscar_BApuntador(disco,sblock,size,ruta,pos,cont);
                    if(r != -1) break;
                }
            }
        }

    }
    return r;
}

int Login::Buscar_BCarpeta(FILE *disco, SuperBloque sblock, int size,  string ruta,int pos,int cont)
{
    //OBTENER LOS NOMBRES DE LAS RUTAS
    char delim[] = "/";
    char nuevo[1000];
    memset(nuevo,'\0',sizeof(nuevo));
    strcat(nuevo,ruta.c_str());

    char* nombres[1000];
    memset(nombres,'\0',sizeof(nombres));

    char *ptr = strtok(nuevo, delim);

    int aux=0;
    while(ptr != NULL)
    {
        nombres [aux++] = ptr;
        ptr = strtok(NULL, delim);
    }

    BloqueCarpeta aux_carpeta;
    int r = -1;

    fseek(disco,Calculo_Posicion_Block(sblock,pos),SEEK_SET);
    fread(&aux_carpeta,sizeof(BloqueCarpeta),1,disco);

    for(int i=0; i<4 ;i++)
    {
        char *name = aux_carpeta.b_content[i].b_name;

        if(strcasecmp(name,nombres[cont]) == 0)
        {
            int lim =0;

            for(int j=0; j<1000 ;j++)//CONTANDO EL NUMERO DE NOMBRES USADOS
            {
                if(nombres[j] != "\0")
                    lim++;
                else
                    break;
            }

            if(cont+1 < lim)
            {
                r = Buscar_Inodo(disco,sblock,size,ruta, aux_carpeta.b_content[i].b_inodo,cont+1 );
                if(r != -1) break;

            }else//CONDICION PARA VERIFICAR QUE LLEGAMOS AL DESTINO
            {
                r = aux_carpeta.b_content[i].b_inodo;
                break;
            }
        }
    }
    return r;
}


int Login::Buscar_BApuntador(FILE *disco, SuperBloque sblock, int size, string ruta, int pos, int cont)
{
    int r = -1;
    BloqueApuntadores aux_apuntador;
    int n = Calcular_N(size);
    char Bm_bloque[3 * n];

    //==================== OBTENGO EL BM DE LOS BLOQUES =============
    fseek(disco,sblock.s_bm_block_start,SEEK_SET);
    fread(Bm_bloque,sizeof(char),3*n,disco);


    fseek(disco,Calculo_Posicion_Block(sblock,pos),SEEK_SET);
    fread(&aux_apuntador,sizeof(BloqueApuntadores),1,disco);

    for(int i=0; i<16 ;i++)
    {
        if(Bm_bloque[ aux_apuntador.b_pointer[i] ] == '1')//SI ES BLOQUE CARPETA
        {
            r = Buscar_BCarpeta(disco,sblock,size,ruta,aux_apuntador.b_pointer[i],cont);
            if(r != -1) break;

        }else if(Bm_bloque[ aux_apuntador.b_pointer[i] ] == '3')//SI ES BLOQUE APUNTADOR
        {
            r = Buscar_BApuntador(disco,sblock,size,ruta,aux_apuntador.b_pointer[i],cont);
            if(r != -1) break;
        }
    }
    return r;
}

int Login::Calculo_Posicion_Block(SuperBloque sblock, int i)
{
    return sblock.s_block_start + (sizeof(BloqueCarpeta) * i );
}

int Login::Calcular_N(int tam_particion)
{
    int tam_SuperBlock = sizeof(SuperBloque);
    int tam_journling = sizeof(Journal);
    int tam_inodo = sizeof(Inodo);
    int tam_block = sizeof(BloqueCarpeta);

    int operador = (tam_particion - tam_SuperBlock);
    int denominador = 1 + tam_journling + 3 + tam_inodo + 3*tam_block;
    int n = floor(operador / denominador);

    return n;
}

int Login::Calculo_Posicion_Inodo(SuperBloque sblock, int i)
{
    return sblock.s_inode_start + (sizeof(Inodo) * i );
}

void Login::Modificar_Inodo(FILE* disco, Inodo inodo, int pos, int tipo)
{
    if(tipo == 0)//ESTE VA SER PARA MODIFICAR FECHA DE LECTURA
    {
        inodo.i_atime = time(0);

    }else if(tipo == 1)//ESTE VA SER PARA MODIFICAR FECHA DE MODIFICACION
    {
        inodo.i_mtime = time(0);
    }


    fseek(disco,pos,SEEK_SET);
    fwrite(&inodo,sizeof(Inodo),1,disco);

}

string Login::Leer_Inodo_Archivo( FILE* disco, SuperBloque sblock,int size , int posI,char* contenido)
{
    Inodo inodo_aux;
    int n = Calcular_N(size);
    char Bm_bloque[3 * n];

    //======================= OBTENGO EL INODO  =====================
    fseek(disco, Calculo_Posicion_Inodo(sblock,posI) ,SEEK_SET); //ME POSICIONO EN EL INODO
    fread(&inodo_aux,sizeof(Inodo),1,disco);

    Modificar_Inodo(disco,inodo_aux,  Calculo_Posicion_Inodo(sblock,posI) ,1);

    //==================== OBTENGO EL BM DE LOS BLOQUES =============
    fseek(disco,sblock.s_bm_block_start,SEEK_SET);
    fread(Bm_bloque,sizeof(char),3*n,disco);

    int index = 0;
    bool r = false;

    for(int i=0;i<16;i++)
    {
        int posB = inodo_aux.i_block[i];
        if(inodo_aux.i_block[i] != -1 )
        {

            if(Bm_bloque[posB] == '2')        //SI ES 2 ES UN BLOQUE CARPETA Ó MAS QUE TODO CUALQUIERA
            {
                r = Leer_BArchivo(disco,sblock,contenido, posB,&index);
                if(r)break;

            }else if(Bm_bloque[posB] == '3')  //SI ES 3 ES UN BLOQUE APUNTADOR
            {
                r = Leer_BApuntador_Archivo(disco, sblock,size, contenido,posB,&index);
                if(r) break;
            }
        }
    }
    return contenido;
}

bool Login::Leer_BArchivo(FILE* disco, SuperBloque sblock, char* contenido, int posA, int* index)
{
    BloqueArchivo archivo_aux;

    char r = false;

    //======================= OBTENGO EL BLOQUE  =====================
    fseek(disco, Calculo_Posicion_Block(sblock,posA) ,SEEK_SET); //ME POSICIONO EN EL INODO
    fread(&archivo_aux,sizeof(BloqueArchivo),1,disco);

    for(int i=0;i<64;i++)
    {
        if(archivo_aux.b_content[i] != '\0')
        {
            contenido[(*index)++]  = archivo_aux.b_content[i];   //SI ES IGUAL ES PORQUE YA NO HAY MAS TEXTO QUE INGRESAR

        }else{r = true; break;}
    }
    return r;
}


bool Login::Leer_BApuntador_Archivo(FILE *disco, SuperBloque sblock,int size, char* contenido,int pos,int* index)
{
    BloqueApuntadores aux_apuntador;
    bool r = false;
    int n = Calcular_N(size);
    char Bm_bloque[3 * n];

    fseek(disco,Calculo_Posicion_Block(sblock,pos),SEEK_SET);
    fread(&aux_apuntador,sizeof(BloqueApuntadores),1,disco);

    //==================== OBTENGO EL BM DE LOS BLOQUES =============
    fseek(disco,sblock.s_bm_block_start,SEEK_SET);
    fread(Bm_bloque,sizeof(char),3*n,disco);

    for(int i=0; i<16 ;i++)
    {
        int apt = aux_apuntador.b_pointer[i];

        if(apt != -1 )    //PRIMERO BUSCAREMOS EN LOS BLOQUES EXISTENTES
        {
            if( Bm_bloque[apt] == '1' ) //EN EL CASO DE BLOQUE CARPETA
            {
                r = Leer_BArchivo(disco,sblock,contenido,apt,index);
                if(r)return true;

            }else if(Bm_bloque[apt] == '3') //EN EL CASO DE BLOQUE APUNTADOR
            {
                r = Leer_BApuntador_Archivo(disco,sblock,size,contenido,apt,index);
                if(r)return true;
            }
        }
    }
    return r;
}
