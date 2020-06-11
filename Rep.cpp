#include "Rep.h"

Rep::Rep(QString name, QString path, string id, QList<Mount> *montajes){
    this->id = id;
    this->name = name;
    this->path = path;
    this->montajes = montajes;
}

void Rep::Hacer_Reporte(){
    bool bandera = false;

    QList<Mount>::iterator i;
    i=i+1;
    string ruta = "";
    for(i = montajes->begin(); i!=montajes->end(); i++ )
    {
        if(this->id == i->id){
            bandera = true;
            ruta = i->path;
            break;
        }
    }

    if(bandera){
        VerificarDirectorio();
        if(this->name == "disk"){

            ReporteDisk(ruta);
        }else if(this->name == "mbr"){
            ReporteMBR(ruta);
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

        struct stat st = {0};
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
