#include "Rep.h"

Rep::Rep(QString name, QString path, string id, QList<Mount> *montajes, string ruta){
    this->id = id;
    this->name = name;
    this->path = path;
    this->montajes = montajes;
    this->posMontada = -1;
    this->ext = "";
    this->ruta = ruta;
}

void Rep::Hacer_Reporte(){
    bool bandera = false;

    QList<Mount>::iterator i;
    i=i+1;
    string ruta = "";
    int pos = 0;
    for(i = montajes->begin(); i!=montajes->end(); i++ )
    {
        if(this->id == i->id){
            bandera = true;
            ruta = i->path;
            this->posMontada = pos;
            break;
        }
        pos++;
    }
    if(bandera){
        VerificarDirectorio();
        this->ext = getExtension(this->path);
        if(this->name == "disk"){
            ReporteDisk(ruta);
        }else if(this->name == "mbr"){
            ReporteMBR(ruta);
        }else if(this->name == "journaling"){
            ReporteJournaling(ruta);
        }else if(this->name == "inode"){
            ReporteInodos(ruta);
        }else if(this->name == "block"){
            ReporteBlock(ruta);
        }else if(this->name == "bminode"){
            ReporteBM_Inode(ruta);
        }else if(this->name == "bmblock"){
            ReporteBM_Block(ruta);
        }else if(this->name == "tree"){
            ReporteTree(ruta);
        }else if(this->name == "sb"){
            ReporteSB(ruta);
        }else if(this->name == "file"){
            ReporteFile(ruta);
        }else if(this->name == "ls"){
            ReporteLS(ruta);
        }else{
            printf("No se reconoce el tipo de reporte que desea\n");
        }

    }else{
        printf("No se encuentra id como particion montada\n");
    }

}


void Rep::ReporteDisk(string ruta){
    FILE* disco;
    MBR mbr;
    disco = fopen(ruta.c_str(),"r+b");

    if(disco == NULL)
    {
        printf("No se pudo abrir el disco\n");
        return;
    }

    fread(&mbr,sizeof(MBR),1,disco);
    char auxRuta[2000];
    memset(auxRuta,'\0',sizeof(auxRuta));
    for(int i=0;i< this->path.size();i++)
    {
        if(i == this->path.size() -4){
            auxRuta[i] = '.';
            auxRuta[i+1] = 'd';
            auxRuta[i+2] = 'o';
            auxRuta[i+3] = 't';
            break;
        }

        auxRuta[i] = this->path.toStdString()[i];
    }
    FILE *grafo = fopen(auxRuta, "wt");
    if(grafo == NULL){
        printf("No se pudo crear el archivo\n");
        return;
    }
    char buffer[2000];
    memset(buffer,'\0',sizeof(buffer));
    string code = "digraph G {\n node [shape=plaintext]\n structMBR [label=<\n<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"6\" COLOR=\"BLACK\">\n";
    fputs(code.c_str(),grafo);

    fputs("<tr>\n<td colspan=\"4\" rowspan=\"4\" bgcolor=\"RED\" >MBR</td>\n",grafo);//AGREGO EL MBR
    int maxSize = mbr.size;
    int recorrido = sizeof(mbr);
    Partition ext;

    int contExt = 0;

    for(int i =0; i<4 ;i++)
    {
        Partition actual = mbr.partitions[i]; //PARTICION ACTUAL

        if(actual.status != '0')//SI ESTA 0 NO ESTA DISPONIBLE, ENTONCES ESTA LIBRE
        {
            if(tolower(actual.type) != 'e')//SI ES EXTENDIDA QUE SE VAYA HACER OTRA COSA
            {
                int porcent;
                int fragmentacion = (actual.start-recorrido);
                if(  fragmentacion > 5 )//EN TEORIA SI ES MAYOR A 0 HAY FRAGMENTACION
                {
                    porcent = (fragmentacion * 100)/maxSize;
                    sprintf(buffer,"<td colspan=\"4\" rowspan=\"4\" bgcolor=\"#b2b7b2\" >Libre<br/> %d%c del disco<br/>%d bytes</td>\n",porcent,'%',fragmentacion);
                    fputs(buffer,grafo);
                    memset(buffer,'\0',sizeof(buffer));
                }
                porcent = (actual.size * 100)/maxSize;
                sprintf(buffer,"<td colspan=\"4\" rowspan=\"4\" bgcolor=\"BLUE\" >%s<br/> (Primaria)<br/> %d%c del disco<br/>%d bytes</td>\n",actual.name,porcent,'%',actual.size);
                fputs(buffer,grafo);
                memset(buffer,'\0',sizeof(buffer));
                recorrido+=actual.size;
            }else
            {
                ext = actual;
                EBR ebr;
                fseek(disco, actual.start ,SEEK_SET);
                fread(&ebr,sizeof(EBR),1,disco);

                int porcent;
                int fragmentacion = (actual.start-recorrido);
                if(  fragmentacion > 5 )//EN TEORIA SI ES MAYOR A 0 HAY FRAGMENTACION
                {
                    porcent = (fragmentacion * 100)/maxSize;
                    //sprintf(buffer,"<td colspan=\"4\" rowspan=\"4\" bgcolor=\"#12d80f\" >Libre<br/> %d%c del disco</td>\n",porcent,'%');
                    sprintf(buffer,"<td colspan=\"4\" rowspan=\"4\" bgcolor=\"#b2b7b2\" >Libre<br/> %d%c del disco<br/>%d bytes</td>\n",porcent,'%',fragmentacion);
                    fputs(buffer,grafo);
                    memset(buffer,'\0',sizeof(buffer));
                }

                while(ebr.next != 0)//INDICA QUE NO HAY MAS
                {
                    contExt+=2;

                    recorrido = ebr.next;
                    fseek(disco, ebr.next ,SEEK_SET);
                    fread(&ebr,sizeof(EBR),1,disco);
                }
                recorrido = ext.size + ext.start; // EN TEORIA ES LA SUMA HASTA EL FINAL DE EXT
                contExt+=2;//AQUI PONER EL RESTO

                sprintf(buffer,"<td colspan=\"%d\" rowspan=\"2\" bgcolor=\"RED\">Extendida</td>\n",2*contExt);
                fputs(buffer,grafo);
                memset(buffer,'\0',sizeof(buffer));


            }
        }
        else
        {
            i++;

            if(i == 4)
            {
                int porcent = ((maxSize-recorrido) * 100)/maxSize;
                if(porcent > 0)
                {
                    sprintf(buffer,"<td colspan=\"4\" rowspan=\"4\" bgcolor=\"BLUE\" >%s<br/> (Primaria)<br/> %d%c del disco<br/>%d bytes</td>\n",actual.name,porcent,'%',maxSize-recorrido);
                    fputs(buffer,grafo);
                    memset(buffer,'\0',sizeof(buffer));//ACTUALIZAR RECORRIDO
                    recorrido = maxSize;
                }
            }else
            {
                while(i<4 )
                {
                    if(mbr.partitions[i].status != '0')// SI ES DISTINTA ENCONTRO FIN
                    {
                        int porcent = ((mbr.partitions[i].start-recorrido) * 100)/maxSize;
                        if(porcent > 0)
                        {
                            //sprintf(buffer,"<td colspan=\"4\" rowspan=\"4\" bgcolor=\"#12d80f\" >Libre<br/> %d%c del disco</td>\n",porcent,'%');
                            sprintf(buffer,"<td colspan=\"4\" rowspan=\"4\" bgcolor=\"#b2b7b2\" >Libre<br/> %d%c del disco<br/>%d bytes</td>\n",porcent,'%',(mbr.partitions[i].start-recorrido));
                            fputs(buffer,grafo);
                            memset(buffer,'\0',sizeof(buffer));
                            recorrido = mbr.partitions[i].start;
                        }
                        i--;
                        break;
                    }
                    else if(i == 3)
                    {
                        int porcent = ((maxSize-recorrido) * 100)/maxSize;
                        if(porcent > 0)
                        {
                            //sprintf(buffer,"<td colspan=\"4\" rowspan=\"4\" bgcolor=\"#12d80f\" >Libre<br/> %d%c del disco</td>\n",porcent,'%');
                            sprintf(buffer,"<td colspan=\"4\" rowspan=\"4\" bgcolor=\"#b2b7b2\" >Libre<br/> %d%c del disco<br/>%d bytes</td>\n",porcent,'%',(maxSize-recorrido));
                            fputs(buffer,grafo);
                            memset(buffer,'\0',sizeof(buffer));
                            recorrido = maxSize;
                        }
                    }else{
                        i--;
                    }
                }
            }

        }
    }
    int resta = maxSize-recorrido;
    int por = resta * 100;
    if(por < 0)
        por = por * -1;
    int porcent = por / maxSize;
    if(porcent >0)
    {
        //sprintf(buffer,"<td colspan=\"4\" rowspan=\"4\" bgcolor=\"#12d80f\" >Libre<br/> %d%c del disco</td>\n",porcent,'%');
        sprintf(buffer,"<td colspan=\"4\" rowspan=\"4\" bgcolor=\"#b2b7b2\" >Libre<br/> %d%c del disco<br/>%d bytes</td>\n",porcent,'%',resta);
        fputs(buffer,grafo);
        memset(buffer,'\0',sizeof(buffer));
    }

    fputs("</tr>\n\n",grafo);

    fputs("<tr>\n<td></td>\n</tr>\n",grafo);

    if(ext.fit != '\0')//EXT GUARDA LA PARTICION EXTENDIDA = ACTUAL
    {
        fputs("<tr>\n",grafo);


        EBR ebr;
        //EBR ebr_anterior;
        int acumulacion = 0; //TAMANIO QUE SE A ESTADO GUARDANDO

        fseek(disco, ext.start ,SEEK_SET);
        fread(&ebr,sizeof(EBR),1,disco);

        while(ebr.next != 0)//INDICA QUE NO HAY MAS
        {
            sprintf(buffer,"<td colspan=\"%d\" rowspan=\"2\" bgcolor=\"RED\">EBR</td>\n",2);
            fputs(buffer,grafo);
            memset(buffer,'\0',sizeof(buffer));

            //int a = (ebr.part_size * 100) / ext.part_size;
            int a = (ebr.size * 100) / maxSize;

            if(a > 0)
            {
                acumulacion+= ebr.size;

                sprintf(buffer,"<td colspan=\"2\" rowspan=\"2\" bgcolor=\"#37e8e6\" >%s<br/> (Logica)<br/> %d%c del disco<br/>%d bytes</td>\n",ebr.name,a,'%',ebr.size);
                fputs(buffer,grafo);
                memset(buffer,'\0',sizeof(buffer));
            }

            //ebr_anterior = ebr;

            recorrido = ebr.next;
            fseek(disco, ebr.next ,SEEK_SET);
            fread(&ebr,sizeof(EBR),1,disco);
        }
        recorrido = ext.size + ext.start;

        int a = ((ext.size - acumulacion ) * 100) / maxSize ; // (SIZE_EXT - SUMA_EXT * 100)/MAXSIZE

        if(a > -1)
        {
            //(buffer,"<td colspan=\"2\" rowspan=\"2\" bgcolor=\"#12d80f\" >LIBRE<br/> %d%c del disco</td>\n",a,'%');
            sprintf(buffer,"<td colspan=\"2\" rowspan=\"2\" bgcolor=\"#b2b7b2\" >LIBRE<br/> %d%c del disco<br/>%d bytes</td>\n",a,'%',(ext.size - acumulacion) );
            fputs(buffer,grafo);
            memset(buffer,'\0',sizeof(buffer));
        }


        fputs("</tr>\n\n",grafo);
    }



    fputs("<tr>\n<td></td>\n</tr>\n",grafo);
    fputs("</TABLE>>];\n}",grafo);

    fclose(grafo);
    fclose(disco);
    sprintf(buffer,"dot -Tpng %s -O ",auxRuta);
    system(buffer); //ESTE PARA CREAR

    printf("Se creo el reporte disk de %s exitosamente\n", this->id.c_str());
}

void Rep::ReporteMBR(string ruta){
    FILE* disco;
    MBR mbr;
    disco = fopen(ruta.c_str(),"r+b");

    if(disco == NULL)
    {
        printf("No se pudo abrir el disco\n");
        fclose(disco);
        return;
    }

    fread(&mbr,sizeof(MBR),1,disco);
    fclose(disco);
    char auxRuta[2000];
    memset(auxRuta,'\0',sizeof(auxRuta));
    for(int i=0;i< this->path.size();i++)
    {
        if(i == this->path.size() -4){
            auxRuta[i] = '.';
            auxRuta[i+1] = 'd';
            auxRuta[i+2] = 'o';
            auxRuta[i+3] = 't';
            break;
        }

        auxRuta[i] = this->path.toStdString()[i];
    }



    VerificarDirectorio(); //CREO LAS CARPETAS NECESARIAS

    FILE* grafo;
    grafo = fopen(auxRuta,"wt");

    if(grafo == NULL){
        printf("Error al crear el archivo\n");
        return;
    }

    //-------------- INICIO DEL CODIGO GRAPHVIZ PARA grafo-----------------
    char buffer[2000];
    memset(buffer,'\0',sizeof(buffer));

    char begin[] = "digraph G {\n node [shape=plaintext]\n structMBR [label=<\n<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\">\n";
    fputs(begin,grafo);
    memset(begin,0,sizeof(begin));

    //-------------- CUERPO DEL GRAPHVIZ DEL grafo-----------------
    fputs("<tr><td colspan=\"2\"><b>MBR</b></td><td></td></tr>",grafo); //NOMBRE
    fputs("<tr>\n<td><b>NOMBRE</b></td>\n<td><b>VALOR</b></td>\n</tr>",grafo); //CABECERA

    sprintf(buffer,"<tr>\n<td><b>mbr_tamaño</b></td>\n<td>%d</td>\n</tr>",mbr.size);
    fputs(buffer,grafo);
    memset(buffer,'\0',sizeof(buffer));

    struct tm *tm;
    char fecha[100];
    tm = localtime(&mbr.creation_time);
    strftime(fecha,100,"%d/%m/%y %H:%S", tm);

    sprintf(buffer,"<tr>\n<td><b>mbr_fecha_creacion</b></td>\n<td>%s</td>\n</tr>",fecha);
    fputs(buffer,grafo);
    memset(buffer,'\0',sizeof(buffer));

    sprintf(buffer,"<tr>\n<td><b>mbr_disk_signature</b></td>\n<td>%d</td>\n</tr>",mbr.disk_signature);
    fputs(buffer,grafo);
    memset(buffer,'\0',sizeof(buffer));

    sprintf(buffer,"<tr>\n<td><b>disk_fit</b></td>\n<td>%c</td>\n</tr>",mbr.disk_fit);
    fputs(buffer,grafo);
    memset(buffer,'\0',sizeof(buffer));

    Partition ext;
    for(int i=0; i<4 ;i++)
    {
        Partition actual = mbr.partitions[i];
        if(actual.status != '0')//SI NO ESTA DISPONIBLE NO GRAFICAR
        {
            sprintf(buffer,"<tr>\n<td><b>part_status_%d</b></td>\n<td>%c</td>\n</tr>",i+1,actual.status);
            fputs(buffer,grafo);
            memset(buffer,'\0',sizeof(buffer));

            sprintf(buffer,"<tr>\n<td><b>part_type_%d</b></td>\n<td>%c</td>\n</tr>",i+1,actual.status);
            fputs(buffer,grafo);
            memset(buffer,'\0',sizeof(buffer));

            sprintf(buffer,"<tr>\n<td><b>part_fit_%d</b></td>\n<td>%c</td>\n</tr>",i+1,actual.fit);
            fputs(buffer,grafo);
            memset(buffer,'\0',sizeof(buffer));

            sprintf(buffer,"<tr>\n<td><b>part_start_%d</b></td>\n<td>%d</td>\n</tr>",i+1,actual.start);
            fputs(buffer,grafo);
            memset(buffer,'\0',sizeof(buffer));

            sprintf(buffer,"<tr>\n<td><b>part_size_%d</b></td>\n<td>%d</td>\n</tr>",i+1,actual.size);
            fputs(buffer,grafo);
            memset(buffer,'\0',sizeof(buffer));

            sprintf(buffer,"<tr>\n<td><b>part_name_%d</b></td>\n<td>%s</td>\n</tr>",i+1,actual.name);
            fputs(buffer,grafo);
            memset(buffer,'\0',sizeof(buffer));

            if( actual.type == 'e' )
            {
                ext = actual;
            }
        }
    }

    if(ext.fit != '\0')
    {
        FILE* disco;
        //MBR mbr;
        disco = fopen(ruta.c_str(),"r+b");

        if(disco == NULL)
        {
            printf("No se pudo abrir el disco\n");
            return;
        }

        EBR ebr;
        fseek(disco, ext.start ,SEEK_SET);
        fread(&ebr,sizeof(EBR),1,disco);

        int i =1;
        while(ebr.next != 0)//INDICA QUE ESTOY EN EL FINAL
        {
            if(ebr.status != '0')
            {
                fputs("</TABLE>>];\n",grafo);
                sprintf(buffer,"structEBR%d [label=<\n<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\">\n",i);
                fputs(buffer,grafo);
                memset(buffer,'\0',sizeof(buffer));

                sprintf(buffer,"<tr><td colspan=\"2\"><b>EBR%d</b></td><td></td></tr>\n",i);//NOMBRE
                fputs(buffer,grafo);
                memset(buffer,'\0',sizeof(buffer));
                fputs("<tr>\n<td><b>NOMBRE</b></td>\n<td><b>VALOR</b></td>\n</tr>\n",grafo); //CABECERA



                sprintf(buffer,"<tr>\n<td><b>part_status_%d</b></td>\n<td>%c</td>\n</tr>",i,ebr.status);
                fputs(buffer,grafo);
                memset(buffer,'\0',sizeof(buffer));

                sprintf(buffer,"<tr>\n<td><b>part_fit_%d</b></td>\n<td>%c</td>\n</tr>",i,ebr.fit);
                fputs(buffer,grafo);
                memset(buffer,'\0',sizeof(buffer));

                sprintf(buffer,"<tr>\n<td><b>part_start_%d</b></td>\n<td>%d</td>\n</tr>",i,ebr.start);
                fputs(buffer,grafo);
                memset(buffer,'\0',sizeof(buffer));

                sprintf(buffer,"<tr>\n<td><b>part_size_%d</b></td>\n<td>%d</td>\n</tr>",i,ebr.size);
                fputs(buffer,grafo);
                memset(buffer,'\0',sizeof(buffer));

                sprintf(buffer,"<tr>\n<td><b>part_next_%d</b></td>\n<td>%d</td>\n</tr>",i,ebr.next);
                fputs(buffer,grafo);
                memset(buffer,'\0',sizeof(buffer));

                sprintf(buffer,"<tr>\n<td><b>part_name_%d</b></td>\n<td>%s</td>\n</tr>",i,ebr.name);
                fputs(buffer,grafo);
                memset(buffer,'\0',sizeof(buffer));

                i++;
            }
            fseek(disco, ebr.next ,SEEK_SET);
            fread(&ebr,sizeof(EBR),1,disco);
        }
        fclose(disco);
    }

    fputs("</TABLE>>];\n}",grafo);
    fclose(grafo);
    sprintf(buffer,"dot -Tpng %s -O ",auxRuta);
    system(buffer);

    printf("Se creo el reporte mbr de %s exitosamente\n", this->id.c_str());
}

void Rep::VerificarDirectorio(){
    char delim[] = "/";
    char nuevo[1000];
    memset(nuevo,'\0',sizeof(nuevo));
    strcat(nuevo,this->path.toLocal8Bit().data());

    char *ptr = strtok(nuevo,delim);


    char* carpetas[100];
    memset(carpetas,'\0',sizeof(carpetas));

    int size=0;
    while(ptr != NULL)
    {
        carpetas[size] = ptr;
        size++;
        ptr = strtok(NULL, delim);
    }

    char auxRuta[1000];
    memset(auxRuta,'\0',sizeof(auxRuta));
    auxRuta[0] = '/';

    for(int i=0; i<size-1 ;i++)
    {
        if(i != 0)
            strcat(auxRuta,"/");

        strcat(auxRuta,carpetas[i]);

        struct stat st= {0};
        string str1 ="";
        if(stat(auxRuta,&st) == -1){
            str1.append("mkdir -p \'");
            str1.append(auxRuta);
            str1.append("\'");
            system(str1.c_str());
            mkdir(auxRuta,0700);
        }

    }
}

void Rep::ReporteJournaling(string ruta){
    int index = -1;
    MBR masterboot;
    SuperBloque super;
    FILE *fp = fopen(ruta.c_str(),"r+b");
    fread(&masterboot,sizeof(MBR),1,fp);
    Mount particionMontada = montajes->at(this->posMontada);
    for(int i = 0; i < masterboot.partitions->size; i++){
        if(masterboot.partitions[i].status =='1'){
            if(strcmp(masterboot.partitions[i].name, particionMontada.name.c_str()) == 0){
                index = i;
                break;
            }
        }
    }
    fseek(fp,masterboot.partitions[index].start,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,fp);
    fclose(fp);
    string aux = this->path.toStdString();
    string delimiter = ".";
    size_t pos = 0;
    while((pos = aux.find(delimiter))!=string::npos){
        aux.erase(0,pos+delimiter.length());
    }
    graficarJournaling(ruta,this->path.toStdString(),aux,masterboot.partitions[index].start);
}

void Rep::graficarJournaling(string direccion, string destino, string extension,int inicio_super){
    FILE *fp = fopen(direccion.c_str(),"r");

    SuperBloque super;
    Journal j;
    fseek(fp,inicio_super,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,fp);

    FILE *graph = fopen("grafica.dot","wt");
    fprintf(graph,"digraph G{\n");
    fprintf(graph, "    nodo [shape=none, fontname=\"Century Gothic\" label=<\n");
    fprintf(graph, "   <table border=\'0\' cellborder='1\' cellspacing=\'0\'>\n");
    fprintf(graph, "    <tr> <td COLSPAN=\'50\' bgcolor=\"cornflowerblue\"> <b>JOURNALING</b> </td></tr>\n");
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"><b>Operacion</b></td> <td bgcolor=\"lightsteelblue\"><b>Nombre</b></td>\n");
    fprintf(graph, "    <td bgcolor=\"lightsteelblue\"><b>Propietario</b></td><td bgcolor=\"lightsteelblue\"><b>Permisos</b></td><td bgcolor=\"lightsteelblue\"><b>Fecha</b></td></tr>\n");
    //
    fseek(fp,inicio_super + static_cast<int>(sizeof(SuperBloque)),SEEK_SET);
    while(ftell(fp) < super.s_bm_inode_start){
        fread(&j,sizeof(Journal),1,fp);
        if(j.journal_type == 1 || j.journal_type == 2){
            struct tm *tm;
            char fecha[100];
            tm = localtime(&j.journal_date);
            strftime(fecha,100,"%d/%m/%y %H:%S",tm);
            fprintf(graph,"<tr><td>%s</td><td>%s</td><td>%d</td><td>%d</td><td>%s</td></tr>\n",j.journal_operation_type,j.operation,j.journal_owner,j.journal_permissions,fecha);
        }
    }
    fprintf(graph, "   </table>>]\n");
    fprintf(graph,"}");
    fclose(graph);

    fclose(fp);

    string comando = "dot -T"+extension+" grafica.dot -o "+destino;
    system(comando.c_str());
    cout << "Reporte Journaling generado con exito " << endl;
}

void Rep::ReporteInodos(string ruta){
    int index = -1;
    MBR masterboot;
    SuperBloque super;
    FILE *fp = fopen(ruta.c_str(),"r+b");
    fread(&masterboot,sizeof(MBR),1,fp);
    Mount particionMontada = montajes->at(this->posMontada);
    for(int i = 0; i < masterboot.partitions->size; i++){
        if(masterboot.partitions[i].status =='1'){
            if(strcmp(masterboot.partitions[i].name, particionMontada.name.c_str()) == 0){
                index = i;
                break;
            }
        }
    }
    fseek(fp,masterboot.partitions[index].start,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,fp);
    fclose(fp);
    string aux = this->path.toStdString();
    string delimiter = ".";
    size_t pos = 0;
    while((pos = aux.find(delimiter))!=string::npos){
        aux.erase(0,pos+delimiter.length());
    }
    graficarInodos(ruta,this->path.toStdString(),aux,super.s_bm_inode_start,super.s_inode_start,super.s_bm_block_start);
}

void Rep::graficarInodos(string direccion, string destino, string extension,int bm_inode_start,int inode_start,int bm_block_start){
    FILE *fp = fopen(direccion.c_str(), "r");

    InodoTable inodo;
    int aux = bm_inode_start;
    int i = 0;
    char buffer;

    FILE *graph = fopen("grafica.dot","w");
    fprintf(graph,"digraph G{\n\n");

    while(aux < bm_block_start){
        fseek(fp,bm_inode_start + i,SEEK_SET);
        buffer = static_cast<char>(fgetc(fp));
        aux++;
        if(buffer == '1'){
            fseek(fp,inode_start + static_cast<int>(sizeof(InodoTable))*i,SEEK_SET);
            fread(&inodo,sizeof(InodoTable),1,fp);
            fprintf(graph, "    nodo_%d [ shape=none fontname=\"Century Gothic\" label=<\n",i);
            fprintf(graph, "   <table border=\'0\' cellborder=\'1\' cellspacing=\'0\' bgcolor=\"royalblue\">");
            fprintf(graph, "    <tr> <td colspan=\'2\'> <b>Inodo %d</b> </td></tr>\n",i);
            fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> i_uid </td> <td bgcolor=\"white\"> %d </td>  </tr>\n",inodo.i_uid);
            fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> i_gid </td> <td bgcolor=\"white\"> %d </td>  </tr>\n",inodo.i_gid);
            fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> i_size </td> <td bgcolor=\"white\"> %d </td> </tr>\n",inodo.i_size);
            struct tm *tm;
            char fecha[100];
            tm=localtime(&inodo.i_atime);
            strftime(fecha,100,"%d/%m/%y %H:%S",tm);
            fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> i_atime </td> <td bgcolor=\"white\"> %s </td>  </tr>\n",fecha);
            tm=localtime(&inodo.i_ctime);
            strftime(fecha,100,"%d/%m/%y %H:%S",tm);
            fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> i_ctime </td> <td bgcolor=\"white\"> %s </td>  </tr>\n",fecha);
            tm=localtime(&inodo.i_mtime);
            strftime(fecha,100,"%d/%m/%y %H:%S",tm);
            fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> i_mtime </td> <td bgcolor=\"white\"> %s </td></tr>\n",fecha);
            for(int b = 0; b < 15; b++)
                fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> i_block_%d </td> <td bgcolor=\"white\"> %d </td> </tr>\n",b,inodo.i_block[b]);
            fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> i_type </td> <td bgcolor=\"white\"> %c </td> </tr>\n",inodo.i_type);
            fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> i_perm </td> <td bgcolor=\"white\"> %d </td> </tr>\n",inodo.i_perm);
            fprintf(graph, "   </table>>]\n");
        }
        i++;
    }
    fprintf(graph,"\n}");
    fclose(graph);

    fclose(fp);

    string comando = "dot -T"+extension+" grafica.dot -o "+destino;
    system(comando.c_str());
    cout << "Reporte de inodos generado con exito " << endl;
}

void Rep::ReporteBlock(string ruta){
    MBR masterboot;
    SuperBloque super;
    FILE *fp = fopen(ruta.c_str(),"r+b");
    fread(&masterboot,sizeof(MBR),1,fp);
    int index = -1;
    Mount particionMontada = montajes->at(this->posMontada);
    for(int i = 0; i < masterboot.partitions->size; i++){
        if(masterboot.partitions[i].status =='1'){
            if(strcmp(masterboot.partitions[i].name, particionMontada.name.c_str()) == 0){
                index = i;
                break;
            }
        }
    }
    fseek(fp,masterboot.partitions[index].start,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,fp);
    fclose(fp);
    graficarBloques(ruta,this->path.toStdString(),ext,super.s_bm_block_start,super.s_block_start,super.s_inode_start);
}

void Rep::graficarBloques(string direccion, string destino, string extension, int bm_block_start, int block_start, int inode_start){
    FILE *fp = fopen(direccion.c_str(),"r");

    BloqueCarpeta carpeta;
    BloqueArchivo archivo;
    BloqueApuntadores apuntador;

    int aux = bm_block_start;
    int i = 0;
    char buffer;

    FILE *graph = fopen("grafica.dot","w");
    fprintf(graph,"digraph G{\n\n");

    while(aux < inode_start){
        fseek(fp,bm_block_start + i,SEEK_SET);
        buffer = static_cast<char>(fgetc(fp));
        aux++;
        if(buffer == '1'){
            fseek(fp,block_start + static_cast<int>(sizeof(BloqueCarpeta))*i,SEEK_SET);
            fread(&carpeta,sizeof(BloqueCarpeta),1,fp);
            fprintf(graph, "    nodo_%d [ shape=none, fontname=\"Century Gothic\" label=< \n",i);
            fprintf(graph, "   <table border=\'0\' cellborder='1' cellspacing='0' bgcolor=\"seagreen\">");
            fprintf(graph, "    <tr> <td colspan=\'2\'> <b>Bloque Carpeta %d</b> </td></tr>\n",i);
            fprintf(graph, "    <tr> <td bgcolor=\"mediumseagreen\"> b_name </td> <td bgcolor=\"mediumseagreen\"> b_inode </td></tr>\n");
            for(int c = 0;c < 4;c++)
                fprintf(graph, "    <tr> <td bgcolor=\"white\"> %s </td> <td bgcolor=\"white\"> %d </td></tr>\n",carpeta.b_content[c].b_name,carpeta.b_content[c].b_inodo);
            fprintf(graph, "   </table>>]\n\n");
        }else if(buffer == '2'){
            fseek(fp,block_start + static_cast<int>(sizeof(BloqueArchivo))*i,SEEK_SET);
            fread(&archivo,sizeof(BloqueArchivo),1,fp);
            fprintf(graph, "    nodo_%d [ shape=none, label=< \n",i);
            fprintf(graph, "   <table border=\'0\' cellborder='1' cellspacing='0' bgcolor=\"sandybrown\">");
            fprintf(graph, "    <tr> <td colspan=\'2\'> <b>Bloque Archivo %d </b></td></tr>\n",i);
            fprintf(graph, "    <tr> <td colspan=\'2\' bgcolor=\"white\"> %s </td></tr>\n",archivo.b_content);
            fprintf(graph, "   </table>>]\n\n");
        }else if(buffer == '3'){
            fseek(fp,block_start + static_cast<int>(sizeof(BloqueApuntadores))*i,SEEK_SET);
            fread(&apuntador,sizeof(BloqueApuntadores),1,fp);
            fseek(fp,block_start + static_cast<int>(sizeof(BloqueApuntadores))*i,SEEK_SET);
            fread(&apuntador,sizeof(BloqueApuntadores),1,fp);
            fprintf(graph, "    bloque_%d [shape=plaintext fontname=\"Century Gothic\" label=< \n",i);
            fprintf(graph, "   <table border=\'0\' bgcolor=\"khaki\">\n");
            fprintf(graph, "    <tr> <td> <b>Pointer block %d</b></td></tr>\n",i);
            for(int j = 0; j < 16; j++)
                fprintf(graph, "    <tr> <td bgcolor=\"white\">%d</td> </tr>\n",apuntador.b_pointer[j]);
            fprintf(graph, "   </table>>]\n\n");
        }
        i++;
    }

    fprintf(graph,"\n}");
    fclose(graph);

    fclose(fp);

    string comando = "dot -T"+extension+" grafica.dot -o "+destino;
    system(comando.c_str());
    cout << "Reporte generado con exito " << endl;
}

void Rep::ReporteBM_Inode(string ruta){
    MBR masterboot;
    SuperBloque super;
    FILE *fp = fopen(ruta.c_str(),"r+b");
    fread(&masterboot,sizeof(MBR),1,fp);
    int index = -1;
    Mount particionMontada = montajes->at(this->posMontada);
    for(int i = 0; i < masterboot.partitions->size; i++){
        if(masterboot.partitions[i].status =='1'){
            if(strcmp(masterboot.partitions[i].name, particionMontada.name.c_str()) == 0){
                index = i;
                break;
            }
        }
    }
    fseek(fp,masterboot.partitions[index].start,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,fp);
    fclose(fp);
    reporteBM(ruta,this->path,super.s_bm_inode_start,super.s_inodes_count);
}

void Rep::reporteBM(string direccion, QString destino, int start_bm, int n){
    FILE *fp = fopen(direccion.c_str(),"r+b");

    char byte;
    if(!destino.endsWith(".txt")){
        destino+=".txt";
    }
    FILE *report = fopen(destino.toStdString().c_str(),"w+");
    fseek(report,0,SEEK_SET);
    int cont = 0;

    for (int i = 0; i < n; i++) {
        fseek(fp,start_bm + i,SEEK_SET);
        byte = static_cast<char>(fgetc(fp));
        if(byte == '0')
            fprintf(report,"0 ");
        else
            fprintf(report,"1 ");
        if(cont == 19){
            cont = 0;
            fprintf(report, "\n");
        }else
            cont++;
    }
    fclose(report);

    fclose(fp);
    cout << "Reporte generado con exito " << endl;
}

void Rep::ReporteBM_Block(string ruta){
    MBR masterboot;
    SuperBloque super;
    FILE *fp = fopen(ruta.c_str(),"r+b");
    fread(&masterboot,sizeof(MBR),1,fp);
    int index = -1;
    Mount particionMontada = montajes->at(this->posMontada);
    for(int i = 0; i < masterboot.partitions->size; i++){
        if(masterboot.partitions[i].status =='1'){
            if(strcmp(masterboot.partitions[i].name, particionMontada.name.c_str()) == 0){
                index = i;
                break;
            }
        }
    }
    fseek(fp,masterboot.partitions[index].start,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,fp);
    fclose(fp);
    reporteBM(ruta,this->path,super.s_bm_block_start,super.s_blocks_count);
}

void Rep::ReporteTree(string ruta){
    MBR masterboot;
    FILE *fp = fopen(ruta.c_str(),"r+b");
    fread(&masterboot,sizeof(MBR),1,fp);
    int index = -1;
    Mount particionMontada = montajes->at(this->posMontada);
    for(int i = 0; i < masterboot.partitions->size; i++){
        if(masterboot.partitions[i].status =='1'){
            if(strcmp(masterboot.partitions[i].name, particionMontada.name.c_str()) == 0){
                index = i;
                break;
            }
        }
    }
    if(index < 0 && index >=4){
        cout << "No se encontro la particion" << endl;
        fclose(fp);
        return;
    }
    fseek(fp,masterboot.partitions[index].start,SEEK_SET);
    fclose(fp);
    graficarTree(ruta,this->path,ext,masterboot.partitions[index].start);
}

void Rep::graficarTree(string direccion, QString destino, string extension, int start_super){
    FILE *fp = fopen(direccion.c_str(),"r");

    SuperBloque super;
    InodoTable inodo;
    BloqueCarpeta carpeta;
    BloqueArchivo archivo;
    BloqueApuntadores apuntador;

    fseek(fp,start_super,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,fp);

    int aux = super.s_bm_inode_start;
    int i = 0;

    char buffer;

    FILE *graph = fopen("grafica.dot", "w");
    fprintf(graph, "digraph G{\n\n");
    fprintf(graph, "    rankdir=\"LR\" \n");

    //Creamos los inodos
    while(aux < super.s_bm_block_start){
        fseek(fp,super.s_bm_inode_start + i,SEEK_SET);
        buffer = static_cast<char>(fgetc(fp));
        aux++;
        int port = 0;
        if(buffer == '1'){
            fseek(fp,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*i,SEEK_SET);
            fread(&inodo,sizeof(InodoTable),1,fp);
            fprintf(graph, "    inodo_%d [ shape=plaintext fontname=\"Century Gothic\" label=<\n",i);
            fprintf(graph, "   <table bgcolor=\"royalblue\" border=\'0\' >");
            fprintf(graph, "    <tr> <td colspan=\'2\'><b>Inode %d</b></td></tr>\n",i);
            fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> i_uid </td> <td bgcolor=\"white\"> %d </td>  </tr>\n",inodo.i_uid);
            fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> i_gid </td> <td bgcolor=\"white\"> %d </td>  </tr>\n",inodo.i_gid);
            fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> i_size </td><td bgcolor=\"white\"> %d </td> </tr>\n",inodo.i_size);
            struct tm *tm;
            char fecha[100];
            tm=localtime(&inodo.i_atime);
            strftime(fecha,100,"%d/%m/%y %H:%S",tm);
            fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> i_atime </td> <td bgcolor=\"white\"> %s </td> </tr>\n",fecha);
            tm=localtime(&inodo.i_ctime);
            strftime(fecha,100,"%d/%m/%y %H:%S",tm);
            fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> i_ctime </td> <td bgcolor=\"white\"> %s </td> </tr>\n",fecha);
            tm=localtime(&inodo.i_mtime);
            strftime(fecha,100,"%d/%m/%y %H:%S",tm);
            fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> i_mtime </td> <td bgcolor=\"white\"> %s </td> </tr>\n",fecha);
            for(int b = 0; b < 15; b++){
                fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> i_block_%d </td> <td bgcolor=\"white\" port=\"f%d\"> %d </td></tr>\n",port,b,inodo.i_block[b]);
                port++;
            }
            fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> i_type </td> <td bgcolor=\"white\"> %c </td>  </tr>\n",inodo.i_type);
            fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> i_perm </td> <td bgcolor=\"white\"> %d </td>  </tr>\n",inodo.i_perm);
            fprintf(graph, "   </table>>]\n\n");
            //Creamos los bloques relacionados al inodo
            for (int j = 0; j < 15; j++) {
                port = 0;
                if(inodo.i_block[j] != -1){
                    fseek(fp,super.s_bm_block_start + inodo.i_block[j],SEEK_SET);
                    buffer = static_cast<char>(fgetc(fp));
                    if(buffer == '1'){//Bloque carpeta
                        fseek(fp,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*inodo.i_block[j],SEEK_SET);
                        fread(&carpeta,sizeof(BloqueCarpeta),1,fp);
                        fprintf(graph, "    bloque_%d [shape=plaintext fontname=\"Century Gothic\" label=< \n",inodo.i_block[j]);
                        fprintf(graph, "   <table bgcolor=\"seagreen\" border=\'0\'>\n");
                        fprintf(graph, "    <tr> <td colspan=\'2\'><b>Folder block %d</b></td></tr>\n",inodo.i_block[j]);
                        fprintf(graph, "    <tr> <td bgcolor=\"mediumseagreen\"> b_name </td> <td bgcolor=\"mediumseagreen\"> b_inode </td></tr>\n");
                        for(int c = 0;c < 4; c++){
                            fprintf(graph, "    <tr> <td bgcolor=\"white\" > %s </td> <td bgcolor=\"white\"  port=\"f%d\"> %d </td></tr>\n",carpeta.b_content[c].b_name,port,carpeta.b_content[c].b_inodo);
                            port++;
                        }
                        fprintf(graph, "   </table>>]\n\n");
                        //Relacion de bloques a inodos
                        for(int c = 0; c < 4; c++){
                            if(carpeta.b_content[c].b_inodo !=-1){
                                if(strcmp(carpeta.b_content[c].b_name,".")!=0 && strcmp(carpeta.b_content[c].b_name,"..")!=0)
                                    fprintf(graph, "    bloque_%d:f%d -> inodo_%d;\n",inodo.i_block[j],c,carpeta.b_content[c].b_inodo);
                            }
                        }
                    }else if(buffer == '2'){//Bloque archivo
                        fseek(fp,super.s_block_start + static_cast<int>(sizeof(BloqueArchivo))*inodo.i_block[j],SEEK_SET);
                        fread(&archivo,sizeof(BloqueArchivo),1,fp);
                        fprintf(graph, "    bloque_%d [shape=plaintext fontname=\"Century Gothic\" label=< \n",inodo.i_block[j]);
                        fprintf(graph, "   <table border=\'0\' bgcolor=\"sandybrown\">\n");
                        fprintf(graph, "    <tr> <td> <b>File block %d</b></td></tr>\n",inodo.i_block[j]);
                        fprintf(graph, "    <tr> <td bgcolor=\"white\"> %s </td></tr>\n",archivo.b_content);
                        fprintf(graph, "   </table>>]\n\n");
                    }else if(buffer == '3'){//Bloque apuntador
                        fseek(fp,super.s_block_start + static_cast<int>(sizeof(BloqueApuntadores))*inodo.i_block[j],SEEK_SET);
                        fread(&apuntador,sizeof(BloqueApuntadores),1,fp);
                        fprintf(graph, "    bloque_%d [shape=plaintext fontname=\"Century Gothic\" label=< \n",inodo.i_block[j]);
                        fprintf(graph, "   <table border=\'0\' bgcolor=\"khaki\">\n");
                        fprintf(graph, "    <tr> <td> <b>Pointer block %d</b></td></tr>\n",inodo.i_block[j]);
                        for(int a = 0; a < 16; a++){
                            fprintf(graph, "    <tr> <td bgcolor=\"white\" port=\"f%d\">%d</td> </tr>\n",port,apuntador.b_pointer[a]);
                            port++;
                        }
                        fprintf(graph, "   </table>>]\n\n");
                        //Bloques carpeta/archivos del bloque de apuntadores
                        for (int x = 0; x < 16; x++) {
                            port = 0;
                            if(apuntador.b_pointer[x] != -1){
                                fseek(fp,super.s_bm_block_start + apuntador.b_pointer[x],SEEK_SET);
                                buffer = static_cast<char>(fgetc(fp));
                                if(buffer == '1'){
                                    fseek(fp,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*apuntador.b_pointer[x],SEEK_SET);
                                    fread(&carpeta,sizeof(BloqueCarpeta),1,fp);
                                    fprintf(graph, "    bloque_%d [shape=plaintext fontname=\"Century Gothic\" label=< \n",apuntador.b_pointer[x]);
                                    fprintf(graph, "   <table border=\'0\' bgcolor=\"seagreen\" >\n");
                                    fprintf(graph, "    <tr> <td colspan=\'2\'> <b>Folder block %d</b> </td></tr>\n",apuntador.b_pointer[x]);
                                    fprintf(graph, "    <tr> <td bgcolor=\"mediumseagreen\"> b_name </td> <td bgcolor=\"mediumseagreen\"> b_inode </td></tr>\n");
                                    for(int c = 0;c < 4; c++){
                                        fprintf(graph, "    <tr> <td bgcolor=\"white\"> %s </td> <td bgcolor=\"white\" port=\"f%d\"> %d </td></tr>\n",carpeta.b_content[c].b_name,port,carpeta.b_content[c].b_inodo);
                                        port++;
                                    }
                                    fprintf(graph, "   </table>>]\n\n");
                                    //Relacion de bloques a inodos
                                    for(int c = 0; c < 4; c++){
                                        if(carpeta.b_content[c].b_inodo !=-1){
                                            if(strcmp(carpeta.b_content[c].b_name,".")!=0 && strcmp(carpeta.b_content[c].b_name,"..")!=0)
                                                fprintf(graph, "    bloque_%d:f%d -> inodo_%d;\n",apuntador.b_pointer[x],c,carpeta.b_content[c].b_inodo);
                                        }
                                    }
                                }else if(buffer == '2'){
                                    fseek(fp,super.s_block_start + static_cast<int>(sizeof(BloqueArchivo))*apuntador.b_pointer[x],SEEK_SET);
                                    fread(&archivo,sizeof(BloqueArchivo),1,fp);
                                    fprintf(graph, "    bloque_%d [shape=plaintext fontname=\"Century Gothic\" label=< \n",apuntador.b_pointer[x]);
                                    fprintf(graph, "   <table border=\'0\' bgcolor=\"sandybrown\">\n");
                                    fprintf(graph, "    <tr> <td> <b>File block %d</b></td></tr>\n",apuntador.b_pointer[x]);
                                    fprintf(graph, "    <tr> <td bgcolor=\"white\"> %s </td></tr>\n",archivo.b_content);
                                    fprintf(graph, "   </table>>]\n\n");
                                }else if(buffer == '3'){

                                }
                            }
                        }

                        //Relacion de bloques apuntador a bloques archivos/carpetas
                        for(int b = 0; b < 16; b++){
                            if(apuntador.b_pointer[b] != -1)
                                fprintf(graph, "    bloque_%d:f%d -> bloque_%d;\n",inodo.i_block[j],b,apuntador.b_pointer[b]);
                        }
                    }
                    //Relacion de inodos a bloques
                    fprintf(graph, "    inodo_%d:f%d -> bloque_%d; \n",i,j,inodo.i_block[j]);
                }
            }
        }
        i++;
    }

    fprintf(graph,"\n\n}");
    fclose(graph);

    fclose(fp);

    string comando = "dot -T"+extension+" grafica.dot -o "+destino.toStdString();
    system(comando.c_str());
    cout << "Reporte Tree generado con exito " << endl;
}

void Rep::ReporteSB(string ruta){
    MBR masterboot;
    FILE *fp = fopen(ruta.c_str(),"r+b");
    fread(&masterboot,sizeof(MBR),1,fp);
    int index = -1;
    Mount particionMontada = montajes->at(this->posMontada);
    for(int i = 0; i < masterboot.partitions->size; i++){
        if(masterboot.partitions[i].status =='1'){
            if(strcmp(masterboot.partitions[i].name, particionMontada.name.c_str()) == 0){
                index = i;
                break;
            }
        }
    }
    fseek(fp,masterboot.partitions[index].start,SEEK_SET);
    fclose(fp);
    graficarSuper(ruta,this->path,ext,masterboot.partitions[index].start);
}

void Rep::graficarSuper(string direccion, QString destino, string extension, int start_super){
    FILE* fp = fopen(direccion.c_str(),"r");

    SuperBloque super;

    fseek(fp,start_super,SEEK_SET);
    fread(&super,sizeof (super),1,fp);

    FILE *graph = fopen("grafica.dot","w");
    fprintf(graph,"digraph G{\n");
    fprintf(graph, "    nodo [shape=none, fontname=\"Century Gothic\" label=<");
    fprintf(graph, "   <table border=\'0\' cellborder='1\' cellspacing=\'0\' bgcolor=\"cornflowerblue\">");
    fprintf(graph, "    <tr> <td COLSPAN=\'2\'> <b>SUPERBLOQUE</b> </td></tr>\n");
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_inodes_count </td> <td bgcolor=\"white\"> %d </td> </tr>\n",super.s_inodes_count);
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_blocks_count </td> <td bgcolor=\"white\"> %d </td> </tr>\n",super.s_blocks_count);
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_free_block_count </td> <td bgcolor=\"white\"> %d </td> </tr>\n",super.s_free_blocks_count);
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_free_inodes_count </td> <td bgcolor=\"white\"> %d </td> </tr>\n",super.s_free_inodes_count);
    struct tm *tm;
    char fecha[100];
    tm=localtime(&super.s_mtime);
    strftime(fecha,100,"%d/%m/%y %H:%S",tm);
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_mtime </td> <td bgcolor=\"white\"> %s </td></tr>\n",fecha);
    tm=localtime(&super.s_umtime);
    strftime(fecha,100,"%d/%m/%y %H:%S",tm);
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_umtime </td> <td bgcolor=\"white\"> %s </td> </tr>\n",fecha);
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_mnt_count </td> <td bgcolor=\"white\"> %d </td> </tr>\n",super.s_mnt_count);
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_magic </td> <td bgcolor=\"white\"> %d </td> </tr>\n",super.s_magic);
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_inode_size </td> <td bgcolor=\"white\"> %d </td> </tr>\n",super.s_inode_size);
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_block_size </td> <td bgcolor=\"white\"> %d </td> </tr>\n",super.s_block_size);
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_first_ino </td> <td bgcolor=\"white\"> %d </td> </tr>\n",super.s_first_ino);
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_first_blo </td> <td bgcolor=\"white\"> %d </td> </tr>\n",super.s_first_blo);
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_bm_inode_start </td> <td bgcolor=\"white\"> %d </td></tr>\n",super.s_bm_inode_start);
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_bm_block_start </td> <td bgcolor=\"white\"> %d </td> </tr>\n",super.s_bm_block_start);
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_inode_start </td> <td bgcolor=\"white\"> %d </td> </tr>\n",super.s_inode_start);
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_block_start </td> <td bgcolor=\"white\"> %d </td> </tr>\n",super.s_block_start);
    fprintf(graph, "   </table>>]\n");
    fprintf(graph,"\n}");
    fclose(graph);

    fclose(fp);

    string comando = "dot -T"+extension+" grafica.dot -o "+destino.toStdString();
    system(comando.c_str());
    cout << "Reporte SuperBloque generado con exito " << endl;
}

void Rep::ReporteFile(string ruta){
    MBR masterboot;
    char auxRuta[500];
    strcpy(auxRuta,this->ruta.c_str());
    FILE *fp = fopen(ruta.c_str(),"r+b");
    fread(&masterboot,sizeof(MBR),1,fp);
    int index = -1;
    Mount particionMontada = montajes->at(this->posMontada);
    for(int i = 0; i < masterboot.partitions->size; i++){
        if(masterboot.partitions[i].status =='1'){
            if(strcmp(masterboot.partitions[i].name, particionMontada.name.c_str()) == 0){
                index = i;
                break;
            }
        }
    }
    int existe = buscarCarpetaArchivo(fp,auxRuta);
    if(existe != -1){
        char nombre[50];
        char auxRuta[400];
        strcpy(auxRuta,this->ruta.c_str());
        strcpy(nombre,basename(auxRuta));
        graficarFILE(ruta,this->path,ext,QString(nombre),masterboot.partitions[index].start,existe);
    }
    fclose(fp);
}

void Rep::graficarFILE(string direccion, QString destino, string extension, QString name,int start_super,int n){
    FILE *fp = fopen(direccion.c_str(),"r");

    SuperBloque super;
    InodoTable inodo;
    BloqueArchivo archivo;

    fseek(fp,start_super,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,fp);
    fseek(fp,super.s_inode_start + static_cast<int>(sizeof(InodoTable))*n,SEEK_SET);
    fread(&inodo,sizeof(InodoTable),1,fp);

    FILE *graph = fopen("grafica.dot","w");
    fprintf(graph,"digraph G{\n");
    fprintf(graph, "    nodo [shape=none, fontname=\"Century Gothic\" label=<");
    fprintf(graph, "   <table border=\'0\' cellborder='1\' cellspacing=\'0\' bgcolor=\"lightsteelblue\">");
    fprintf(graph, "    <tr><td align=\"left\"> <b>%s</b> </td></tr>\n",name.toStdString().c_str());
    fprintf(graph, "    <tr><td bgcolor=\"white\">");
    for (int i = 0; i < 15; i++) {
        if(inodo.i_block[i] != -1){
            if(i == 12){//Apuntador indirecto simple
                BloqueApuntadores apuntador;
                fseek(fp,super.s_block_start + static_cast<int>(sizeof(BloqueApuntadores))*inodo.i_block[i],SEEK_SET);
                fread(&apuntador,sizeof(BloqueApuntadores),1,fp);
                for(int j = 0; j < 16; j++){
                    if(apuntador.b_pointer[j] != -1){
                        fseek(fp,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*apuntador.b_pointer[j],SEEK_SET);
                        fread(&archivo,sizeof(BloqueArchivo),1,fp);
                        fprintf(graph,"%s <br/>",archivo.b_content);
                    }
                }
            }else if(i == 13){

            }else if(i == 14){

            }else{//Apuntadores directos
                fseek(fp,super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*inodo.i_block[i],SEEK_SET);
                fread(&archivo,sizeof(BloqueArchivo),1,fp);
                fprintf(graph,"%s <br/>",archivo.b_content);
            }
        }
    }
    fprintf(graph, "    </td></tr>\n");
    fprintf(graph, "   </table>>]\n");
    fprintf(graph,"\n}");
    fclose(graph);

    fclose(fp);

    string comando = "dot -T"+extension+" grafica.dot -o "+destino.toStdString();
    system(comando.c_str());
    cout << "Reporte file generado con exito " << endl;
}

void Rep::ReporteLS(string ruta){
    MBR masterboot;
    SuperBloque super;
    InodoTable inodo;
    char auxRuta[500];
    strcpy(auxRuta,this->ruta.c_str());
    FILE *fp = fopen(daLoguer.direccion.c_str(),"r+b");
    fread(&masterboot,sizeof(MBR),1,fp);
    int index = -1;
    Mount particionMontada = montajes->at(this->posMontada);
    for(int i = 0; i < masterboot.partitions->size; i++){
        if(masterboot.partitions[i].status =='1'){
            if(strcmp(masterboot.partitions[i].name, particionMontada.name.c_str()) == 0){
                index = i;
                break;
            }
        }
    }
    fseek(fp,masterboot.partitions[index].start,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,fp);

    FILE *graph = fopen("grafica.dot","w");
    fprintf(graph,"digraph G{\n\n");
    fprintf(graph, "    nodo [ shape=none, fontname=\"Century Gothic\" \n");
    fprintf(graph, "    label=< <table border=\'0\' cellborder='1\' cellspacing=\'0\' bgcolor=\"lightsteelblue\">\n");
    fprintf(graph, "     <tr> <td><b>Permisos</b></td><td><b>Owner</b></td><td><b>Grupo</b></td><td><b>Size</b></td><td><b>Fecha</b></td><td><b>Hora</b></td><td><b>Tipo</b></td><td><b>Name</b></td> </tr>\n");

    char nombre[50];
    strcpy(auxRuta,this->ruta.c_str());
    strcpy(nombre,basename(auxRuta));
    fseek(fp,super.s_inode_start + static_cast<int>(sizeof(InodoTable)),SEEK_SET);
    fread(&inodo,sizeof(InodoTable),1,fp);
    Usuario user = getUsuario(daLoguer.direccion,inodo.i_uid);
    graficarPermisos(graph, fp,0,user);

    fprintf(graph, "    </table>>]\n");
    fprintf(graph,"\n}");
    fclose(graph);

    fclose(fp);

    string comando = "dot -T"+ext+" grafica.dot -o "+ this->path.toStdString();
    system(comando.c_str());
    cout << "Reporte ls generado con exito " << endl;
}

void Rep::graficarPermisos(FILE* graph, FILE* fp, int inodePos, Usuario user){

    SuperBloque sb;
    fseek(fp, daLoguer.inicioSuper, SEEK_SET);
    fread(&sb, sizeof (SuperBloque), 1, fp);

    Inodo inodo;
    int inodeSize = static_cast<int>(sizeof (Inodo));
    fseek(fp, sb.s_inode_start + inodeSize*inodePos, SEEK_SET);
    fread(&inodo, sizeof (Inodo), 1, fp);

    for(int i=0; i < 15; i++){
        if(inodo.i_block[i] == -1)
            continue;

        if(i < 12){
            if(inodo.i_type == '0'){
                BloqueCarpeta fb;
                int folderSize = static_cast<int>(sizeof (BloqueCarpeta));
                fseek(fp, sb.s_block_start + folderSize*inodo.i_block[i], SEEK_SET);
                fread(&fb, sizeof (BloqueCarpeta), 1, fp);

                string auxS = "";
                for(int i = 0; i < 4; i++){
                    auxS = fb.b_content[i].b_name;
                    if((auxS.length() > 1 && auxS[0] != '.') || (auxS.length() > 1 && auxS[1] != '.')){

                        string auxPermisos = to_string(inodo.i_perm);
                        char propietario = auxPermisos[0];
                        char grupo = auxPermisos[1];
                        char otros = auxPermisos[2];
                        char permisos[50];

                        //Tipo de permisos para el propietario
                        if(propietario == '0')
                            strcpy(permisos,"---");
                        else if(propietario == '1')
                            strcpy(permisos,"--x");
                        else if(propietario == '2')
                            strcpy(permisos,"-w-");
                        else if(propietario == '3')
                            strcpy(permisos,"-wx");
                        else if(propietario == '4')
                            strcpy(permisos,"r--");
                        else if(propietario == '5')
                            strcpy(permisos,"r-x");
                        else if(propietario == '6')
                            strcpy(permisos,"rw-");
                        else if(propietario == '7')
                            strcpy(permisos,"rwx");

                        //Tipo de permisos para grupo
                        if(grupo == '0')
                            strcat(permisos," ---");
                        else if(grupo == '1')
                            strcat(permisos," --x");
                        else if(grupo == '2')
                            strcat(permisos," -w-");
                        else if(grupo == '3')
                            strcat(permisos," -wx");
                        else if(grupo == '4')
                            strcat(permisos," r--");
                        else if(grupo == '5')
                            strcat(permisos," r-x");
                        else if(grupo == '6')
                            strcat(permisos," rw-");
                        else if(grupo == '7')
                            strcat(permisos," rwx");

                        //Tipo de permisos para otros
                        if(otros == '0')
                            strcat(permisos," ---");
                        else if(otros == '1')
                            strcat(permisos," --x");
                        else if(otros == '2')
                            strcat(permisos," -w-");
                        else if(otros == '3')
                            strcat(permisos," -wx");
                        else if(otros == '4')
                            strcat(permisos," r--");
                        else if(otros == '5')
                            strcat(permisos," r-x");
                        else if(otros == '6')
                            strcat(permisos," rw-");
                        else if(otros == '7')
                            strcat(permisos," rwx");

                        fprintf(graph,"<tr> <td bgcolor=\"white\">%s</td> ",permisos);
                        fprintf(graph, "<td bgcolor=\"white\">%s</td>",user.username);
                        fprintf(graph, "<td bgcolor=\"white\">%s</td>",user.group);
                        fprintf(graph, "<td bgcolor=\"white\">%d</td>",inodo.i_size);

                        struct tm *tm;
                        char fecha[100];
                        tm=localtime(&inodo.i_atime);
                        strftime(fecha,100,"%d/%m/%y",tm);
                        fprintf(graph, "<td bgcolor=\"white\">%s</td>",fecha);
                        strftime(fecha,100,"%H:%S",tm);
                        fprintf(graph,"<td bgcolor=\"white\">%s</td>",fecha);
                        if(auxS.find(".")==string::npos)
                            fprintf(graph,"<td bgcolor=\"white\">%s</td>","Carpeta");
                        else
                            fprintf(graph,"<td bgcolor=\"white\">%s</td>","Archivo");
                        fprintf(graph, "<td bgcolor=\"white\">%s</td> </tr>\n", auxS.c_str());
                        graficarPermisos(graph, fp,fb.b_content[i].b_inodo,user);
                    }
                }
            }
        }
    }
}



int Rep::byteInodoBloque(FILE *stream,int pos, char tipo){
    SuperBloque super;
    fseek(stream,daLoguer.inicioSuper,SEEK_SET);
    fread(&super,sizeof(SuperBloque),1,stream);
    if(tipo == '1'){
        return (super.s_inode_start + static_cast<int>(sizeof(InodoTable))*pos);
    }else if(tipo == '2')
        return (super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta))*pos);
    return 0;
}

int Rep::buscarCarpetaArchivo(FILE *stream, char* path){
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

string Rep::getExtension(QString direccion){
    string aux = direccion.toStdString();
    string delimiter = ".";
    size_t pos = 0;
    while((pos = aux.find(delimiter))!=string::npos){
        aux.erase(0,pos+delimiter.length());
    }
    return aux;
}


Usuario Rep::getUsuario(string direccion,int usuario){
    FILE *fp = fopen(direccion.c_str(),"r+b");

    char cadena[400] = "\0";
    SuperBloque super;
    InodoTable inodo;
    Usuario response;

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
        char grupo[12];
        char *end_token;
        char *token2 = strtok_r(token,",",&end_token);
        strcpy(id,token2);
        if(strcmp(id,"0") != 0){//Verificar que no sea un U/G eliminado
            token2 = strtok_r(nullptr,",",&end_token);
            strcpy(tipo,token2);
            if(strcmp(tipo,"U") == 0){
                token2 = strtok_r(nullptr,",",&end_token);
                strcpy(grupo,token2);
                token2 = strtok_r(nullptr,",",&end_token);
                strcpy(user,token2);
                int idAux = atoi(id);
                if(idAux == usuario){
                    string groupName(grupo);
                    response.id_usr = atoi(id);
                    response.id_grp = buscarGrupo(groupName);
                    strcpy(response.username,user);
                    strcpy(response.group,grupo);
                    return response;
                }

            }
        }
        token = strtok_r(nullptr,"\n",&end_str);
    }
    return response;
}

int Rep::buscarGrupo(string name){
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

