#include "MKFS.h"

MKFS::MKFS(string id, string type, QList<Mount> *montajes)
{
    this->id = id;
    this->type = type;
    this->montajes = montajes;
    this->ruta = "";
}

void MKFS::Ejecutar(){
    if(VerificarMontaje(this->id)){
        if(this->type == "fast"){
            FormatFast(this->ruta);
        }else if (this->type=="full"){
            FormatFull(this->ruta);
        }else{
            cout << "No se reconoce el tipo de formateo que desea realizar!" << endl;
        }
    }else{
        cout << "Debe estar montada la particion que desea formatear!" << endl;
    }
}

bool MKFS::VerificarMontaje(string id){
    QList<Mount>::iterator i;
    for(i = montajes->begin(); i!=montajes->end(); i++ )
    {
        if(i->id == id)
        {
            this->ruta = i->path;
            return true;
        }
    }
    return false;
}

void MKFS::FormatFast(string path){
    FILE *disco = fopen(path.c_str(), "r+b");
    if(disco == NULL){
        printf("Error al abrir el disco!");
        return;
    }
    Mount particionMontada = getMontaje(this->id);
    if(particionMontada.id == "vacio"){
        cout << "No se pudo formatear el disco" << endl;
        fclose(disco);
        return;
    }
    SuperBloque sblock;
    fseek(disco, particionMontada.inicio, SEEK_SET);
    fread(&sblock, sizeof (SuperBloque), 1, disco);
    int n = Calcular_N(particionMontada.size);

    if(sblock.s_block_size == 0)//SI ES VERDADERO NO HAY SUPER BLOCK, SI ES FALSE ES PORQUE YA EXITE SUPER BLOCK
    {

        LLenar_SBlock(&sblock, n,particionMontada.inicio);

        char bitmap_inodo[n];       //BITMAP DE INODOS
        memset(bitmap_inodo,'0',sizeof(bitmap_inodo));
        char bitmap_bloque[3*n];    //BITMAP DE BLOQUES
        memset(bitmap_bloque,'0',sizeof(bitmap_bloque));

        //LLENAR LA PARTICION CON LOS INODOS, BLOQUES Y LOS BITMAP'S
        fseek(disco,particionMontada.inicio,SEEK_SET);
        fwrite(&sblock,sizeof(SuperBloque),1,disco);

        fseek(disco,sblock.s_bm_inode_start,SEEK_SET);
        fwrite(bitmap_inodo,sizeof(bitmap_inodo),1,disco);

        fseek(disco,sblock.s_bm_block_start,SEEK_SET);
        fwrite(bitmap_bloque,sizeof(bitmap_bloque),1,disco);

    }else
    {
        sblock.s_first_blo = 0;
        sblock.s_first_ino = 0;
        sblock.s_free_blocks_count = 3 * n * sizeof(BloqueCarpeta);
        sblock.s_free_inodes_count = n * sizeof(Inodo);


        char bitmap_inodo[n];       //BITMAP DE INODOS
        memset(bitmap_inodo,'0',sizeof(bitmap_inodo));
        char bitmap_bloque[3*n];    //BITMAP DE BLOQUES
        memset(bitmap_bloque,'0',sizeof(bitmap_bloque));


        //LLENAR LA PARTICION CON LOS INODOS, BLOQUES Y LOS BITMAP'S
        fseek(disco,particionMontada.inicio,SEEK_SET);
        fwrite(&sblock,sizeof(SuperBloque),1,disco);

        fseek(disco,sblock.s_bm_inode_start,SEEK_SET);
        fwrite(bitmap_inodo,sizeof(bitmap_inodo),1,disco);

        fseek(disco,sblock.s_bm_block_start,SEEK_SET);
        fwrite(bitmap_bloque,sizeof(bitmap_bloque),1,disco);

    }

    fclose(disco);

    Crear_Raiz(ruta,sblock,particionMontada);//AQUI CREO LA CARPETA RAIZ (/)

}

void MKFS::FormatFull(string path){
    FILE *disco = fopen(path.c_str(), "r+b");
    if(disco == NULL){
        printf("Error al abrir el disco!");
        return;
    }
    Mount particionMontada = getMontaje(this->id);
    if(particionMontada.id == "vacio"){
        cout << "No se pudo formatear el disco" << endl;
        fclose(disco);
        return;
    }
    SuperBloque sblock;
    fseek(disco,particionMontada.inicio,SEEK_SET);
    fread(&sblock,sizeof(SuperBloque),1,disco);//SE LEE EL DISCO CON EL TAMAÑO DEL SUPERBLOQUE

    int n = Calcular_N(particionMontada.size);

    if(sblock.s_block_size == 0)//SI ES VERDADERO NO HAY SUPER BLOCK, SI ES FALSE ES PORQUE YA EXITE SUPER BLOCK
    {

        LLenar_SBlock(&sblock, n,particionMontada.inicio);

        char bitmap_inodo[n];       //BITMAP DE INODOS
        memset(bitmap_inodo,'0',sizeof(bitmap_inodo));
        char bitmap_bloque[3*n];    //BITMAP DE BLOQUES
        memset(bitmap_bloque,'0',sizeof(bitmap_bloque));

        //LLENAR LA PARTICION CON LOS INODOS, BLOQUES Y LOS BITMAP'S
        fseek(disco,particionMontada.inicio,SEEK_SET);
        fwrite(&sblock,sizeof(SuperBloque),1,disco);

        fseek(disco,sblock.s_bm_inode_start,SEEK_SET);
        fwrite(bitmap_inodo,sizeof(bitmap_inodo),1,disco);

        fseek(disco,sblock.s_bm_block_start,SEEK_SET);
        fwrite(bitmap_bloque,sizeof(bitmap_bloque),1,disco);

    }else
    {
        sblock.s_first_blo = 0;
        sblock.s_first_ino = 0;
        sblock.s_free_blocks_count = 3 * n ;
        sblock.s_free_inodes_count = n ;


        char bitmap_inodo[n];       //BITMAP DE INODOS
        memset(bitmap_inodo,'0',sizeof(bitmap_inodo));
        char bitmap_bloque[3*n];    //BITMAP DE BLOQUES
        memset(bitmap_bloque,'0',sizeof(bitmap_bloque));


        //LLENAR LA PARTICION CON LOS INODOS, BLOQUES Y LOS BITMAP'S
        fseek(disco,particionMontada.inicio,SEEK_SET);
        fwrite(&sblock,sizeof(SuperBloque),1,disco);

        fseek(disco,sblock.s_bm_inode_start,SEEK_SET);
        fwrite(bitmap_inodo,sizeof(bitmap_inodo),1,disco);

        fseek(disco,sblock.s_bm_block_start,SEEK_SET);
        fwrite(bitmap_bloque,sizeof(bitmap_bloque),1,disco);

        //LIMPIAR LAS SECCIONES DE INODOS Y BLOQUES
        char inodo[n*sizeof(Inodo)];
        memset(inodo,'\0',sizeof(inodo));
        fseek(disco,sblock.s_inode_start,SEEK_SET);
        fwrite(inodo,sizeof(inodo),1,disco);

        char block[3*n*sizeof(BloqueCarpeta)];
        memset(block,'\0',sizeof(block));
        fseek(disco,sblock.s_block_start,SEEK_SET);
        fwrite(block,sizeof(block),1,disco);

    }

    fclose(disco);

    Crear_Raiz(ruta,sblock,particionMontada);//AQUI CREO LA CARPETA RAIZ (/)
}

Mount MKFS::getMontaje(string id){
    QList<Mount>::iterator i;
    int pos = 0;
    for(i = montajes->begin(); i!=montajes->end(); i++ )
    {
        if(i->id == id)
        {
            return montajes->at(pos);
        }
        pos++;
    }
    return *new Mount("","","vacio",0);
}

int MKFS::Calcular_N(int size){
    int tam_SuperBlock = sizeof(SuperBloque);
    int tam_journling = sizeof(Journal);
    int tam_inodo = sizeof(Inodo);
    int tam_block = sizeof(BloqueCarpeta);

    int operador = (size - tam_SuperBlock);
    int denominador = 1 + tam_journling + 3 + tam_inodo + 3*tam_block;
    int n = floor(operador / denominador);

    return n;
}

void MKFS::Crear_Raiz(string ruta, SuperBloque sb, Mount mount){
    FILE* disco;

    disco = fopen(ruta.c_str(),"r+b");

    if(disco == NULL){
        printf("Al abrir el discon");
        return;
    }

    // Crear el inodo raiz
    Inodo inodo;
    inodo.i_uid = 1;
    inodo.i_gid = 1;
    inodo.i_size = 0;
    inodo.i_atime = time(0);
    inodo.i_type = '0';
    inodo.i_ctime = time(0);
    inodo.i_perm = 777;


    memset(inodo.i_block,-1,sizeof(inodo.i_block));

    inodo.i_block[0] = 0;

    fseek(disco, Calculo_Posicion_Inodo(sb,0),SEEK_SET);
    fwrite(&inodo, sizeof(Inodo), 1 ,  disco);


    // Crear capeta raiz
    BloqueCarpeta b_carpeta;

    for(int i=0; i<15 ;i++)
        b_carpeta.b_content[i].b_inodo = -1;


    memset(b_carpeta.b_content[0].b_name,'\0',sizeof(b_carpeta.b_content[0].b_name));
    strcpy(b_carpeta.b_content[0].b_name, ".");
    b_carpeta.b_content[0].b_inodo = 0;

    memset(b_carpeta.b_content[1].b_name,'\0',sizeof(b_carpeta.b_content[1].b_name));
    strcpy(b_carpeta.b_content[1].b_name, "..");
    b_carpeta.b_content[1].b_inodo = 0;


    fseek(disco, Calculo_Posicion_Block(sb,0) ,SEEK_SET);
    fwrite(&b_carpeta, sizeof(BloqueCarpeta), 1 ,  disco);

    Marcar_BitMap_I(sb.s_bm_inode_start,mount.size, disco, '1', 0);
    Marcar_BitMap_B(sb.s_bm_block_start,mount.size, disco, '1', 0);

    Modificar_Count_I(&sb,mount.inicio, disco, 0,mount.size);
    Modificar_Count_B(&sb,mount.inicio, disco, 0, mount.size);

    // Inodo para el archivo txt
    Inodo inodoA;
    inodoA.i_uid = 1;
    inodoA.i_gid = 1;
    inodoA.i_size = 52;
    inodoA.i_atime = time(0);
    inodoA.i_ctime = time(0);
    inodoA.i_mtime = time(0);
    inodoA.i_type = '1';
    inodoA.i_perm = 777;

    memset(inodoA.i_block,-1,sizeof(inodoA.i_block));


    inodoA.i_block[0] = 1;


    fseek(disco, Calculo_Posicion_Inodo(sb,1) ,SEEK_SET);
    fwrite(&inodoA,sizeof(Inodo),1,disco);

    // Actualizar inodos
    Marcar_BitMap_I(sb.s_bm_inode_start,mount.size, disco, '1', 1);
    Modificar_Count_I(&sb,mount.inicio,disco,0,mount.size);


    //Enlazar inodo nuevo
    memset(b_carpeta.b_content[2].b_name,'\0',sizeof(b_carpeta.b_content[0].b_name));
    strcpy(b_carpeta.b_content[2].b_name, "users.txt");
    b_carpeta.b_content[2].b_inodo = 1;

    fseek(disco, Calculo_Posicion_Block(sb,0) ,SEEK_SET);
    fwrite(&b_carpeta, sizeof(BloqueCarpeta), 1 ,  disco);


    // Crear Bloque Archivo
    BloqueArchivo b_archivo;
    memset(b_archivo.b_content,'\0',sizeof(b_archivo.b_content));

    strcpy(b_archivo.b_content , "1,G,root      \n1,U,root      ,root      ,123       \n");

    fseek(disco, Calculo_Posicion_Block(sb,1) ,SEEK_SET);
    fwrite(&b_archivo, sizeof(BloqueArchivo), 1 ,  disco);

    //Actualizar Bloque archivos
    Marcar_BitMap_B(sb.s_bm_block_start,mount.size, disco, '2', 1);
    Modificar_Count_B(&sb,mount.inicio, disco, 0, mount.size);

    fclose(disco);
}


void MKFS::LLenar_SBlock(SuperBloque *sb, int n, int inicio){
    sb->s_filesystem_type = 3;
    sb->s_inodes_count = n ;
    sb->s_blocks_count = 3 * n ;
    sb->s_free_blocks_count = 3 * n ;
    sb->s_free_inodes_count = n ;
    sb->s_mtime = time(0);
    sb->s_umtime = time(0);
    sb->s_mnt_count = sb->s_mnt_count +1;
    sb->s_magic = 61267 ;
    sb->s_inode_size = sizeof(Inodo);
    sb->s_block_size = sizeof(BloqueCarpeta);
    sb->s_first_ino = 0;
    sb->s_first_blo = 0;
    sb->s_bm_inode_start = inicio + sizeof(SuperBloque)+ 1 + (n * sizeof(Journal)) +1;
    sb->s_bm_block_start = sb->s_bm_inode_start + n + 1 ;
    sb->s_inode_start = sb->s_bm_block_start + (3*n) + 1 ;
    sb->s_block_start = sb->s_inode_start + n * sizeof(Inodo) +1 ;
}

int MKFS::Calculo_Posicion_Inodo(SuperBloque sb, int i)
{
    return sb.s_inode_start + (sizeof(Inodo) * i );
}

int MKFS::Calculo_Posicion_Block(SuperBloque sb, int i)
{
    return sb.s_block_start + (sizeof(BloqueCarpeta) * i );
}

void MKFS::Modificar_Count_I(SuperBloque *sb, int inicio, FILE* disco, int tipo, int inode_start)
{
    sb->s_first_ino = BM_Inodo_Disponible(disco,sb->s_bm_inode_start,inode_start);

    if(tipo == 0)
    {
        sb->s_free_inodes_count = sb->s_free_inodes_count -1;

    }else if(tipo == 1)
    {
        sb->s_free_inodes_count = sb->s_free_inodes_count +1;
    }
    fseek(disco,inicio,SEEK_SET);
    fwrite(sb, sizeof(SuperBloque), 1, disco);
}


void MKFS::Modificar_Count_B(SuperBloque *sb, int inicio, FILE* disco, int tipo, int block_start)
{
    sb->s_first_blo = BM_Bloque_Disponible(disco,sb->s_bm_inode_start,block_start);

    if(tipo == 0)
    {
        sb->s_free_blocks_count = sb->s_free_blocks_count -1;

    }else if(tipo == 1)
    {
        sb->s_free_blocks_count = sb->s_free_blocks_count +1;
    }
    fseek(disco,inicio,SEEK_SET);
    fwrite(sb, sizeof(SuperBloque), 1, disco);
}

void MKFS::Marcar_BitMap_I(int inode_start, int size, FILE* disco, char a, int posicion)
{
    fseek(disco,inode_start,SEEK_SET);
    int n = Calcular_N(size);

    char Bm_inodo[n];
    fread(Bm_inodo,sizeof(char), n,disco);

    Bm_inodo[posicion] = a;
    fseek(disco,inode_start,SEEK_SET);
    fwrite(Bm_inodo,sizeof(Bm_inodo),1,disco);

}

void MKFS::Marcar_BitMap_B(int block_start,int size, FILE* disco, char a, int posicion)
{
    fseek(disco,block_start,SEEK_SET);
    int n = Calcular_N(size);

    char Bm_bloque[3*n];
    fread(Bm_bloque,sizeof(char), 3*n,disco);

    Bm_bloque[posicion] = a;
    fseek(disco,block_start,SEEK_SET);
    fwrite(Bm_bloque,sizeof(Bm_bloque),1,disco);
}


int MKFS::BM_Inodo_Disponible(FILE* disco,int inode_start,int size)
{
    fseek(disco,inode_start,SEEK_SET);
    int n = Calcular_N(size);
    char Bm_inodo[n];
    fread(Bm_inodo,sizeof(char),n,disco);

    for(unsigned int i=0; i < sizeof(Bm_inodo) ;i++)
    {
        if(Bm_inodo[i] == '0')
            return i;
    }
    return -1;
}

int MKFS::BM_Bloque_Disponible(FILE* disco,int block_start,int size)
{
    fseek(disco,block_start,SEEK_SET);
    int n = Calcular_N(size);

    char Bm_bloque[3*n];
    fread(Bm_bloque,sizeof(char), 3*n,disco);


    for(unsigned int i=0; i < sizeof(Bm_bloque) ;i++)
    {
        if(Bm_bloque[i] == '0')
            return i;
    }
    return -1;
}



void MKFS::Modificar_Inodo(FILE* disco, Inodo inodo, int pos, int tipo)
{
    if(tipo == 0)
    {
        inodo.i_atime = time(0);
    }else if(tipo == 1)
    {
        inodo.i_mtime = time(0);
    }
    fseek(disco,pos,SEEK_SET);
    fwrite(&inodo,sizeof(Inodo),1,disco);
}
