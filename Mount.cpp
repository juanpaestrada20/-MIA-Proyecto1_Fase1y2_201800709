#include "Mount.h"

Mount::Mount(string name, string path, string id , QList<Mount> *montajes)
{
    this->name = name;
    this->path = path;
    this->id = id;
    this->letra = '\0';
    this->num = 0;
    this->montajes = montajes;
}

void Mount::Ejecutar(){
    if(VerificarMontaje()){
        printf("Ya esta montada la particion\n");
        return;
    }

    MBR mbr;
    FILE *disco = fopen(this->path.c_str(), "r+b");

    if(disco== NULL){
        printf("No se puede abrir el disco, verifique la ruta\n");
        return;
    }
    fread(&mbr, sizeof (MBR), 1, disco);

    if(Nombre_noRepetido(mbr, disco)){
        Partition part;
        bool bandera = false;
        for(int i=0; i<4 ;i++)
        {
            part = mbr.partitions[i];

            if(part.type == 'e') //EN DADO CASO DE QUE SEA EXTENDIDA
            {

                if(part.name == this->name)
                {
                    bandera = true;
                    break;
                }

                fseek(disco,part.start,SEEK_SET); //ME POSICIONO EN DONDE COMIENZA LAS LOGICAS

                int finPart = part.size + part.start;
                EBR ebr;
                int corrida = 0;
                int pesoEBR = sizeof(EBR);
                fread(&ebr, sizeof (EBR), 1, disco);//obtener el ebr inicial
                corrida = pesoEBR + ebr.start;
                while(corrida < finPart){
                    if(ebr.name == this->name && ebr.status != '0'){
                        bandera = true;
                        break;
                    }else{
                        if(ebr.next != 0){// si no hay siguiente es porque es el final de la lista
                            int espacio = ebr.next;
                            corrida = pesoEBR +espacio;
                            fseek(disco, espacio, SEEK_SET);
                            fread(&ebr, sizeof(EBR), 1, disco);
                        }else{
                            break;
                        }
                    }
                }
            }else //SI NO EN DADO CASO DE QUE SEA PRIMARIA
            {
                if(part.name == this->name)
                {
                    bandera = true;
                    break;
                }
            }
        }
        Mount *aux = Get_Mount(this->path);
        if(bandera)
        {
            if(aux->num == 0)//SI ES 0 NO ESTA EL DISCO DISCO, SI ES != 0 ESE DISCO YA ESTA
            {
                aux->num = 1;
                string letra = getLetra();
                string nombre = setID(letra,aux->num);
                Mount *nuevo = new Mount(this->name, this->path, nombre, 0);
                nuevo->letra=letra[0];
                nuevo->num=1;
                montajes->append(*nuevo);
                std::cout << "Se montó la particion " << nombre << endl;

            }else
            {
                int numero = aux->num+1;
                string letra = getLetra();
                string nombre = setID(letra, numero);
                Mount *nuevo = new Mount(this->name, this->path, nombre, 0);
                nuevo->letra=letra[0];
                nuevo->num=numero;
                montajes->append(*nuevo);
                std::cout << "Se montó la particion " << nombre << endl;


            }

        }else{
            if(aux->num == 0)//SI ES 0 NO ESTA EL DISCO DISCO, SI ES != 0 ESE DISCO YA ESTA
                        {
                            aux->num = 1;
                            string letra = getLetra();
                            string nombre = setID(letra,aux->num);
                            Mount *nuevo = new Mount(this->name, this->path, nombre, 0);
                            nuevo->letra=letra[0];
                            nuevo->num=1;
                            montajes->append(*nuevo);
                            std::cout << "Se montó la particion " << nombre << endl;

                        }else
                        {
                            int numero = aux->num+1;
                            string letra = getLetra();
                            string nombre = setID(letra, numero);
                            Mount *nuevo = new Mount(this->name, this->path, nombre, 0);
                            nuevo->letra=letra[0];
                            nuevo->num=numero;
                            montajes->append(*nuevo);
                            std::cout << "Se montó la particion " << nombre << endl;


                        }
        }
    }else{
        printf("No se encontro la particion \n");
    }
    fclose(disco);
}

bool Mount::Nombre_noRepetido(MBR mbr, FILE *disco){
    for(int i = 0; i < 4; i++){
        Partition part = mbr.partitions[i];
        // En el caso que la particion se expandida
        if(part.type == 'e'){
            if(part.name == this->name && part.status != '0'){
                return true;
            }
            fseek(disco, part.start, SEEK_SET); // Posicion donde comienzan las logicas
            int finPart = part.size + part.start;
            EBR ebr;
            int corrida = 0;
            int pesoEBR = sizeof(EBR);
            fread(&ebr, sizeof (EBR), 1, disco);//obtener el ebr inicial
            corrida = pesoEBR + ebr.start;
            while(corrida < finPart){
                if(strcmp(ebr.name, this->name.c_str())==0 && ebr.status != '0'){
                    return true;
                }else{
                    if(ebr.next != 0){// si no hay siguiente es porque es el final de la lista
                        int espacio = ebr.next;
                        corrida = pesoEBR +espacio;
                        fseek(disco, espacio, SEEK_SET);
                        fread(&ebr, sizeof(EBR), 1, disco);
                    }else{
                        break;
                    }
                }
            }
        }else{
            // si es primaria
            if(part.name == this->name && part.status != '0'){
                return true;
            }
        }
    }
    return false;
}

Mount* Mount::Get_Mount(string path)
{
    Mount *aux = new Mount(this->name,this->path, "", 0);
    aux->num = 0;

    if(montajes->isEmpty()){
        return aux;
    }
    int a = 0;
    QList<Mount>::iterator i;
    i=i+1;
    for(i = montajes->begin(); i!=montajes->end(); i++ )
    {
        if(i->path == path)
        {
            a++;
        }
    }
    aux->num=a;

    return aux;
}

string Mount::setID(string letra, int num){
    string r = "vd" + letra + std::to_string(num);
    return r;
}

string Mount::getLetra(){
    char a = 'a';
    if(montajes->size() == 0){
        return "a";
    }
    QList<Mount>::iterator i;
    for(i=montajes->begin(); i!=montajes->end(); i++)
    {

        if((int)i->letra >= (int)a){
            a++;
        }
        if(i->path == this->path)
        {
            string letr ="";
            letr = i->letra;
            return letr;
        }

    }
    string r ="";
    r+=a;
    return r;
}

bool Mount::VerificarMontaje(){
    QList<Mount>::iterator i;
    i=i+1;
    for(i = montajes->begin(); i!=montajes->end(); i++ )
    {
        if(i->path == path && i->name == this->name)
        {
            return true;
        }
    }
    return false;
}
