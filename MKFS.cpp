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
    int inicio;
    int tamano;
    int index = -1;
    MBR masterboot;
    fread(&masterboot, sizeof (MBR),1, disco);
    for(int i = 0; i < masterboot.partitions->size; i++){
        if(masterboot.partitions[i].status =='1'){
            if(strcmp(masterboot.partitions[i].name, particionMontada.name.c_str()) == 0){
                index = i;
                break;
            }
        }
    }
    inicio = masterboot.partitions[index].start;
    tamano = masterboot.partitions[index].size;
    int n = Calcular_N(tamano);//numero de estructuras
    int n_bloques = 3*n;
    int super_size = sizeof (SuperBloque);
    int journal_size = sizeof (Journal) * n;
    SuperBloque sblock;
    sblock = LLenar_SBlock(n, n_bloques, super_size, journal_size, inicio);

    Inodo inodo;
    BloqueCarpeta bloque;
    char buffer = '0';
    char buffer2 = '1';
    char buffer3 = '2';

    fseek(disco,inicio,SEEK_SET);
        fwrite(&sblock,sizeof(SuperBloque),1,disco);
        /*----------------BITMAP DE INODOS----------------*/
        for(int i = 0; i < n; i++){
            fseek(disco,sblock.s_bm_inode_start + i,SEEK_SET);
            fwrite(&buffer,sizeof(char),1,disco);
        }
        /*----------bit para / y users.txt en BM----------*/
        fseek(disco,sblock.s_bm_inode_start,SEEK_SET);
        fwrite(&buffer2,sizeof(char),1,disco);
        fwrite(&buffer2,sizeof(char),1,disco);
        /*---------------BITMAP DE BLOQUES----------------*/
        for(int i = 0; i < n_bloques; i++){
            fseek(disco,sblock.s_bm_block_start + i,SEEK_SET);
            fwrite(&buffer,sizeof(char),1,disco);
        }
        /*----------bit para / y users.txt en BM----------*/
        fseek(disco,sblock.s_bm_block_start,SEEK_SET);
        fwrite(&buffer2,sizeof(char),1,disco);
        fwrite(&buffer3,sizeof(char),1,disco);
        /*------------inodo para carpeta root-------------*/
        inodo.i_uid = 1;
        inodo.i_gid = 1;
        inodo.i_size = 0;
        inodo.i_atime = time(nullptr);
        inodo.i_ctime = time(nullptr);
        inodo.i_mtime = time(nullptr);
        inodo.i_block[0] = 0;
        for(int i = 1; i < 15;i++){
            inodo.i_block[i] = -1;
        }
        inodo.i_type = '0';
        inodo.i_perm = 664;
        fseek(disco,sblock.s_inode_start,SEEK_SET);
        fwrite(&inodo,sizeof(InodoTable),1,disco);
        /*------------Bloque para carpeta root------------*/
        strcpy(bloque.b_content[0].b_name,".");//Actual
        bloque.b_content[0].b_inodo=0;

        strcpy(bloque.b_content[1].b_name,"..");//Padre
        bloque.b_content[1].b_inodo=0;

        strcpy(bloque.b_content[2].b_name,"users.txt");
        bloque.b_content[2].b_inodo=1;

        strcpy(bloque.b_content[3].b_name,".");
        bloque.b_content[3].b_inodo=-1;
        fseek(disco,sblock.s_block_start,SEEK_SET);
        fwrite(&bloque,sizeof(BloqueCarpeta),1,disco);
        /*-------------inodo para users.txt-------------*/
        inodo.i_uid = 1;
        inodo.i_gid = 1;
        inodo.i_size = 27;
        inodo.i_atime = time(nullptr);
        inodo.i_ctime = time(nullptr);
        inodo.i_mtime = time(nullptr);
        inodo.i_block[0] = 1;
        for(int i = 1; i < 15;i++){
            inodo.i_block[i] = -1;
        }
        inodo.i_type = '1';
        inodo.i_perm = 755;
        fseek(disco,sblock.s_inode_start + static_cast<int>(sizeof(InodoTable)),SEEK_SET);
        fwrite(&inodo,sizeof(InodoTable),1,disco);
        /*-------------Bloque para users.txt------------*/
        BloqueArchivo archivo;
        memset(archivo.b_content,0,sizeof(archivo.b_content));
        strcpy(archivo.b_content,"1,G,root\n1,U,root,root,123\n");
        fseek(disco,sblock.s_block_start + static_cast<int>(sizeof(BloqueCarpeta)),SEEK_SET);
        fwrite(&archivo,sizeof(BloqueArchivo),1,disco);

        cout << "Disco formateado en EXT3" << endl;

        fclose(disco);

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
    int inicio;
    int tamano;
    int index = -1;
    MBR masterboot;
    fread(&masterboot, sizeof (MBR),1, disco);
    for(int i = 0; i < masterboot.partitions->size; i++){
        if(masterboot.partitions[i].status =='1'){
            if(strcmp(masterboot.partitions[i].name, particionMontada.name.c_str()) == 0){
                index = i;
                break;
            }
        }
    }
    inicio = masterboot.partitions[index].start;
    tamano = masterboot.partitions[index].size;
    int n = Calcular_N(tamano);//numero de estructuras
    int n_bloques = 3*n;
    int super_size = sizeof (SuperBloque);
    int journal_size = sizeof (Journal) * n;
    SuperBloque sblock;
    sblock = LLenar_SBlock(n, n_bloques, super_size, journal_size, inicio);

    Inodo inodo;
    BloqueCarpeta bloque;
    char buffer = '0';
    char buffer2 = '1';
    char buffer3 = '2';

    fseek(disco,inicio,SEEK_SET);
        fwrite(&sblock,sizeof(SuperBloque),1,disco);
        /*----------------BITMAP DE INODOS----------------*/
        for(int i = 0; i < n; i++){
            fseek(disco,sblock.s_bm_inode_start + i,SEEK_SET);
            fwrite(&buffer,sizeof(char),1,disco);
        }
        /*----------bit para / y users.txt en BM----------*/
        fseek(disco,sblock.s_bm_inode_start,SEEK_SET);
        fwrite(&buffer2,sizeof(char),1,disco);
        fwrite(&buffer2,sizeof(char),1,disco);
        /*---------------BITMAP DE BLOQUES----------------*/
        for(int i = 0; i < n_bloques; i++){
            fseek(disco,sblock.s_bm_block_start + i,SEEK_SET);
            fwrite(&buffer,sizeof(char),1,disco);
        }
        /*----------bit para / y users.txt en BM----------*/
        fseek(disco,sblock.s_bm_block_start,SEEK_SET);
        fwrite(&buffer2,sizeof(char),1,disco);
        fwrite(&buffer3,sizeof(char),1,disco);
        /*------------inodo para carpeta root-------------*/
        inodo.i_uid = 1;
        inodo.i_gid = 1;
        inodo.i_size = 0;
        inodo.i_atime = time(nullptr);
        inodo.i_ctime = time(nullptr);
        inodo.i_mtime = time(nullptr);
        inodo.i_block[0] = 0;
        for(int i = 1; i < 15;i++){
            inodo.i_block[i] = -1;
        }
        inodo.i_type = '0';
        inodo.i_perm = 664;
        fseek(disco,sblock.s_inode_start,SEEK_SET);
        fwrite(&inodo,sizeof(InodoTable),1,disco);
        /*------------Bloque para carpeta root------------*/
        strcpy(bloque.b_content[0].b_name,".");//Actual
        bloque.b_content[0].b_inodo=0;

        strcpy(bloque.b_content[1].b_name,"..");//Padre
        bloque.b_content[1].b_inodo=0;

        strcpy(bloque.b_content[2].b_name,"users.txt");
        bloque.b_content[2].b_inodo=1;

        strcpy(bloque.b_content[3].b_name,".");
        bloque.b_content[3].b_inodo=-1;
        fseek(disco,sblock.s_block_start,SEEK_SET);
        fwrite(&bloque,sizeof(BloqueCarpeta),1,disco);
        /*-------------inodo para users.txt-------------*/
        inodo.i_uid = 1;
        inodo.i_gid = 1;
        inodo.i_size = 27;
        inodo.i_atime = time(nullptr);
        inodo.i_ctime = time(nullptr);
        inodo.i_mtime = time(nullptr);
        inodo.i_block[0] = 1;
        for(int i = 1; i < 15;i++){
            inodo.i_block[i] = -1;
        }
        inodo.i_type = '1';
        inodo.i_perm = 755;
        fseek(disco,sblock.s_inode_start + static_cast<int>(sizeof(InodoTable)),SEEK_SET);
        fwrite(&inodo,sizeof(InodoTable),1,disco);
        /*-------------Bloque para users.txt------------*/
        BloqueArchivo archivo;
        memset(archivo.b_content,0,sizeof(archivo.b_content));
        strcpy(archivo.b_content,"1,G,root\n1,U,root,root,123\n");
        fseek(disco,sblock.s_block_start + static_cast<int>(sizeof(BloqueCarpeta)),SEEK_SET);
        fwrite(&archivo,sizeof(BloqueArchivo),1,disco);

        cout << "Disco formateado en EXT3" << endl;

        fclose(disco);

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
    int denominador = 4 + tam_journling  + tam_inodo + 3*tam_block;
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
    inodo.i_mtime= time(0);
    inodo.i_perm = 664;


    memset(inodo.i_block,-1,sizeof(inodo.i_block));

    inodo.i_block[0] = 0;

    fseek(disco, sb.s_inode_start,SEEK_SET);
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

    memset(b_carpeta.b_content[2].b_name,'\0',sizeof(b_carpeta.b_content[1].b_name));
    strcpy(b_carpeta.b_content[2].b_name, "users.txt");
    b_carpeta.b_content[2].b_inodo = 0;

    memset(b_carpeta.b_content[3].b_name,'\0',sizeof(b_carpeta.b_content[1].b_name));
    strcpy(b_carpeta.b_content[3].b_name, ".");
    b_carpeta.b_content[3].b_inodo = -1;

    fseek(disco, sb.s_block_start ,SEEK_SET);
    fwrite(&b_carpeta, sizeof(BloqueCarpeta), 1 ,  disco);

    // Inodo para el archivo txt
    Inodo inodoA;
    inodoA.i_uid = 1;
    inodoA.i_gid = 1;
    inodoA.i_size = 27;
    inodoA.i_atime = time(0);
    inodoA.i_ctime = time(0);
    inodoA.i_mtime = time(0);
    inodoA.i_type = '1';
    inodoA.i_perm = 755;

    memset(inodoA.i_block,-1,sizeof(inodoA.i_block));


    inodoA.i_block[0] = 1;


    fseek(disco, sb.s_inode_start + sizeof (Inodo),SEEK_SET);
    fwrite(&inodoA,sizeof(Inodo),1,disco);

    // Crear Bloque Archivo
    BloqueArchivo b_archivo;
    memset(b_archivo.b_content,'\0',sizeof(b_archivo.b_content));

    strcpy(b_archivo.b_content , "1,G,root\n1,U,root,root,123\n");

    fseek(disco,sb.s_block_start + sizeof (BloqueCarpeta) ,SEEK_SET);
    fwrite(&b_archivo, sizeof(BloqueArchivo), 1 ,  disco);

    fclose(disco);

    cout << "Disco formateado en EXT3" << endl;
}


SuperBloque MKFS::LLenar_SBlock(int num_estructuras, int num_bloques, int super_size, int journal_size, int inicio){
    SuperBloque super;
    super.s_filesystem_type = 3;
    super.s_inodes_count = num_estructuras;
    super.s_blocks_count = num_bloques;
    super.s_free_blocks_count = num_bloques - 2;
    super.s_free_inodes_count = num_estructuras - 2;
    super.s_mtime = time(nullptr);
    super.s_umtime = 0;
    super.s_mnt_count = 0;
    super.s_magic = 0xEF53;
    super.s_inode_size = sizeof(InodoTable);
    super.s_block_size = sizeof(BloqueArchivo);
    super.s_first_ino = 2;
    super.s_first_blo = 2;
    super.s_bm_inode_start = inicio + super_size + journal_size;
    super.s_bm_block_start = inicio + super_size + journal_size + num_estructuras;
    super.s_inode_start = inicio + super_size + journal_size + num_estructuras + num_bloques;
    super.s_block_start = inicio + super_size + journal_size + num_estructuras + num_bloques + sizeof(Inodo)*num_estructuras;

    return super;
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
