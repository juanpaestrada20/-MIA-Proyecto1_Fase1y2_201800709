#include "Interprete.h"


using namespace std;


Interprete::Interprete(NodoAST *raiz)
{
    this->raiz = raiz;
    this->size = 1;
    this->fitPredeterminadoM = "ff"; // Predeterminado par MkDisk
    this->fitPredeterminadoF = "wf"; //Predeterminado par FDisk
    this->unitPredeterminada = 'm';
    this->path = "";
    this->typePredeterminado = 'p';
    this->deletePredeterminado = "";
    this->name = "";
    this->add = 0;
    this->ID = "";
    this->error = false;
    this->opcion_fdisk = 0;
    this->user = "";
    this->pass = "";
    this->grupo = "";
    this->ugo = 0;
    this->recursivo = false;
    this->carpetaPadre = false;
    this->cont = "";
    this->dest = "";
}

void Interprete::ejecutar(){
    int cantHijos = raiz->hijos.count();
    for(int i = 0; i < cantHijos; i++){
    Recorrer_Arbol(&raiz->hijos[i]);
    }
}

void Interprete::Recorrer_Arbol(NodoAST *raiz){
    QString tipoComando = raiz->valor;
    if(tipoComando == "MkDisk"){
        Opciones_Parametro(&raiz->hijos[0], 0);
        if(!this->error){
            int tam = this->size;
            char fit;
            fit = this->fitPredeterminadoM.toLower().toStdString()[0];
            char unidad = this->unitPredeterminada.toLower().toStdString()[0];
            QString ruta = this->path;

            MKDisk *DiscoNuevo = new MKDisk(tam, fit, unidad, ruta);
            DiscoNuevo->Ejecutar();
        }
        restorePred();
    } else if(tipoComando == "RmDisk"){
        Opciones_Parametro(raiz, 1);
        if(!this->error){
            RMDisk *DiscoEliminar = new RMDisk(path);
            DiscoEliminar->Ejecutar();
        }
        restorePred();
    } else if(tipoComando == "FDisk"){
        Opciones_Parametro(&raiz->hijos[0], 2);
        if(!this->error){
            FDisk *F_Disk = new FDisk(this->size,this->unitPredeterminada.toLower().toStdString()[0], this->path, this->typePredeterminado ,this->fitPredeterminadoF, this->deletePredeterminado, this->name.toStdString(), this->add, this->opcion_fdisk);
            F_Disk->Ejecutar();
        }
        restorePred();
    } else if(tipoComando == "Mount"){
        Opciones_Parametro(&raiz->hijos[0], 3);
        if(!this->error){
            Mount *Montar = new Mount(this->name.toStdString(), this->path.toStdString(), this->ID.toStdString(), montajes);
            Montar->Ejecutar();
        }
        restorePred();
    } else if(tipoComando == "Unmount"){
        Opciones_Parametro(raiz, 4);
        if(!this->error){
            EliminarMount(this->ID);
        }
        restorePred();
    } else if(tipoComando == "Rep"){
        Opciones_Parametro(&raiz->hijos[0], 5);
        if(!this->error){
            Rep *rep = new Rep(this->name, this->path, this->ID.toStdString(), montajes);
            rep->Hacer_Reporte();
        }
        restorePred();
    } else if(tipoComando == "MkFs"){
        Opciones_Parametro(&raiz->hijos[0], 6);
        if(!this->error){
//            MKFS *format = new MKFS();
//            format->Ejecutar();
        }
        restorePred();
    }else if (tipoComando == "Login"){
        Opciones_Parametro(&raiz->hijos[0], 7);
        if(!this->error){
//            Login *login = new Login();
//            login->Ejecutar();
        }
        restorePred();
    }else if (tipoComando == "Logout"){
        if(!this->error){
//            Login *login = new Login();
//            login->Ejecutar();
        }
        restorePred();
    }else if (tipoComando == "MkGrp"){
        if(raiz->hijos[0].valor.toStdString() == "Name"){
            this->grupo = raiz->hijos[0].hijos[0].valor.toStdString();
        }else{
            cout << "Debe indicar el nombre del grupo para agregar" << endl;
            this->error= true;
        }
        if(!this->error){
//            Login *login = new Login();
//            login->Ejecutar();
        }
        restorePred();
    }else if (tipoComando == "RkGrp"){
        if(raiz->hijos[0].valor.toStdString() == "Name"){
            this->grupo = raiz->hijos[0].hijos[0].valor.toStdString();
        }else{
            cout << "Debe indicar el nombre del grupo para eliminar" << endl;
            this->error= true;
        }
        if(!this->error){
//            Login *login = new Login();
//            login->Ejecutar();
        }
        restorePred();
    }else if (tipoComando == "MkUsr"){
        Opciones_Parametro(&raiz->hijos[0], 8);
        if(!this->error){
//            MKUser *user = new MKUser();
//            user->Ejecutar();
        }
        restorePred();
    }else if (tipoComando == "RmUsr"){
        if(raiz->hijos[0].valor.toStdString() == "Usr"){
            this->user = raiz->hijos[0].hijos[0].valor.toStdString();
        }else{
            cout << "Debe indicar el nombre del usuario para eliminar" << endl;
            this->error= true;
        }
        if(!this->error){
//            RMUser *user = new RMUser();
//            user->Ejecutar();
        }
        restorePred();
    }else if (tipoComando == "Chmod"){
        Opciones_Parametro(&raiz->hijos[0], 9);
        if(!this->error){
//            Chmod *chmod = new Chmod();
//            chmod->Ejecutar();
        }
        restorePred();
    }else if (tipoComando == "MKFile"){
        Opciones_Parametro(&raiz->hijos[0], 10);
        if(!this->error){
//            MKFile *mkfile = new MKFile();
//            mkfile->Ejecutar();
        }
        restorePred();
    }else if (tipoComando == "Cat"){
        if(raiz->hijos[0].valor.toStdString() == "File"){
            this->path = raiz->hijos[0].hijos[0].valor;
        }else{
            cout << "Debe indicar la ruta del archivo" << endl;
            this->error= true;
        }
        if(!this->error){
//            Cat *cat = new Cat();
//            cat->Ejecutar();
        }
        restorePred();
    }else if (tipoComando == "Rem"){
        if(raiz->hijos[0].valor.toStdString() == "Path"){
            this->path = raiz->hijos[0].hijos[0].valor;
        }else{
            cout << "Debe indicar el nombre del usuario para eliminar" << endl;
            this->error= true;
        }
        if(!this->error){
//            Rem *rem = new Rem();
//            rem->Ejecutar();
        }
        restorePred();
    }else if (tipoComando == "Edit"){
        Opciones_Parametro(&raiz->hijos[0], 11);
        if(!this->error){
//            Edit *edit = new Edit();
//            edit->Ejecutar();
        }
        restorePred();
    }else if (tipoComando == "Ren"){
        Opciones_Parametro(&raiz->hijos[0], 12);
        if(!this->error){
//            Edit *edit = new Edit();
//            edit->Ejecutar();
        }
        restorePred();
    }else if (tipoComando == "MkDir"){
        Opciones_Parametro(&raiz->hijos[0], 13);
        if(!this->error){
//            Edit *edit = new Edit();
//            edit->Ejecutar();
        }
        restorePred();
    }else if (tipoComando == "Cp"){
        Opciones_Parametro(&raiz->hijos[0], 14);
        if(!this->error){
//            Edit *edit = new Edit();
//            edit->Ejecutar();
        }
        restorePred();
    }else if (tipoComando == "Mv"){
        Opciones_Parametro(&raiz->hijos[0], 14);
        if(!this->error){
//            Edit *edit = new Edit();
//            edit->Ejecutar();
        }
        restorePred();
    }else if (tipoComando == "Find"){
        Opciones_Parametro(&raiz->hijos[0], 12);
        if(!this->error){
//            Edit *edit = new Edit();
//            edit->Ejecutar();
        }
        restorePred();
    }else if (tipoComando == "Chown"){
        Opciones_Parametro(&raiz->hijos[0], 15);
        if(!this->error){
//            Edit *edit = new Edit();
//            edit->Ejecutar();
        }
        restorePred();
    }else if (tipoComando == "ChGrp"){
        Opciones_Parametro(&raiz->hijos[0], 16);
        if(!this->error){
//            Edit *edit = new Edit();
//            edit->Ejecutar();
        }
        restorePred();
    }else if (tipoComando == "Pause"){
        if(!this->error){
//            Edit *edit = new Edit();
//            edit->Ejecutar();
        }
        restorePred();
    }


}

void Interprete::Opciones_Parametro(NodoAST *raiz, int tipo){
    switch (tipo) {
    // MkDisk
    case 0:
    {
        int cantParametros = raiz->hijos.count();
        int size = 0;
        char unit = 'n';
        QString path = "";
        char fit = '0';
        QString parametro;

        for(int i = 0; i < cantParametros; i++){
            parametro=raiz->hijos[i].valor;
         if(parametro == "Size"){
             size = raiz->hijos[i].hijos[0].valor.toInt();
         } else if(parametro == "Unit"){
             unit = raiz->hijos[i].hijos[0].valor.toStdString()[0];
         } else if(parametro == "Ruta"){
             path = raiz->hijos[i].hijos[0].valor;
         } else if(parametro == "Fit"){
             fit = raiz->hijos[i].hijos[0].valor.toLower().toStdString()[0];
         }
        }
        // si hay algun parametro obligatorio faltante
        if (size > 0){
            this->size = size;
        }else{
            this->error = true;
            printf("Tamaño de Disco no permitido\n");
        }
        if(path != ""){
            this->path = path;
        }else{
            this->error = true;
            printf("Ubicacion para crear disco no especificada\n");
        }
        if(unit=='n'){
            this->unitPredeterminada = "m";
        }else{
            if(unit=='k'){
                this->unitPredeterminada = "k";
            }else if(unit=='m'){
                this->unitPredeterminada = "m";
            }else if(unit=='b'){
                this->unitPredeterminada = "b";
            }else{
                this->error = true;
                printf("No se reconoce el tipo de unidad ingresado\n");
            }
        }
        if(fit!='0'){
            if(fit=='f'){
                this->fitPredeterminadoM = 'f';
            }
            else if(fit=='w'){
                this->fitPredeterminadoM ='w';
            }
            else if(fit=='b'){
                this->fitPredeterminadoM = 'b';
            }else{
                this->error= true;
                printf("No se reconoce el fit\n");
            }
        }
        break;
    }
    // RmDisk
    case 1:
    {
        int cantParametros = raiz->hijos.count();
        QString path = "";
        QString parametro;
        for(int i = 0; i < cantParametros; i++){
            parametro=raiz->hijos[i].valor;
         if(parametro == "Path"){
             path = raiz->hijos[i].hijos[0].valor;
         }
        }
         if(path == ""){
             printf("Debe indicar la ruta para eliminar el disco!\n");
             this->error = true;
         }else{
             this->path=path;
         }
        break;
    }
    // FDisk
    case 2:
    {
        int cantParametros = raiz->hijos.count();
        string parametro;
        int size=0;
        char unit='k';
        string path="";
        char type='p';
        string fit="wf";
        string deleteType ="";
        string name="";
        int add=0;

        for(int i = 0; i < cantParametros; i++){
            parametro=raiz->hijos[i].valor.toStdString();
         if(parametro == "Size"){
             size = raiz->hijos[i].hijos[0].valor.toInt();
         } else if(parametro == "Unit"){
             unit = raiz->hijos[i].hijos[0].valor.toStdString()[0];
         } else if(parametro == "Path"){
             path = raiz->hijos[i].hijos[0].valor.toStdString();
         } else if(parametro == "Type"){
             type = raiz->hijos[i].hijos[0].valor.toLower().toStdString()[0];
         }else if(parametro == "Fit"){
             fit[0] = raiz->hijos[i].hijos[0].valor.toLower().toStdString()[0];
             fit[1] = raiz->hijos[i].hijos[0].valor.toLower().toStdString()[1];
         }else if(parametro == "Delete"){
             deleteType = raiz->hijos[i].hijos[0].valor.toStdString();
         }
         else if(parametro == "Name"){
             name = raiz->hijos[i].hijos[0].valor.toStdString();
         }else if(parametro == "Add"){
             add = raiz->hijos[i].hijos[0].valor.toInt();
         }
        }

        //verificar errores
        if(path!=""){
            this->path = QString(path.c_str());
        }else{
            printf("Debe indicar la ruta!\n");
            this->error = true;
        }
        if(name!=""){
            this->name = QString(name.c_str());
        }else{
            printf("Debe indicar el nombre de la particion!\n");
            this->error = true;
        }
        if(size!=0){
            if(deleteType != "" || add!=0){
                printf("No se puede utilizar los parametos delete y/o add cuando se esta creando una particion!\n");
                this->error = true;
            }else{
                if(size>0){
                    this->size = size;
                }else{
                    printf("El tamaño de la particion debe ser mayor a 0\n");
                    this->error = true;
                }
            }
        }else{
            if(deleteType != "" && add!=0){
                printf("No se puede eliminar y agregar espacio al mismo tiempo\n");
                this->error = true;
            }else if(deleteType != "" && add==0){
                this->deletePredeterminado = QString(deleteType.c_str());
            }else if(deleteType == "" && add != 0){
                this->opcion_fdisk = 1;
                this->add = add;
            }else{
                printf("Indique que realizara con el disco\n");
                this->error = true;
            }
        }
        if(unit!='k'){
            if(unit=='m'){
                this->unitPredeterminada = "m";
            }else if(unit=='b'){
                this->unitPredeterminada = "b";
            }else{
                this->error = true;
                printf("No se reconoce el tipo de unidad ingresado\n");
            }
        }else{
            this->unitPredeterminada = "k";
        }
        if(type!='p'){
            if(type=='e'){
                this->typePredeterminado = 'e';
            }else if (type=='l'){
                this->typePredeterminado = 'l';
            }else{
                this->error = true;
                printf("Error de tipo de particion\n");
            }
        }else{
            this->typePredeterminado='p';
        }
        if(fit[0]!='w' && fit[1]!='f'){
            if(fit[0]=='f' && fit[1]=='f'){
                fitPredeterminadoM = fit[0] +fit[1];
            }
            else if(fit[0]=='w' && fit[1]=='f'){
                fitPredeterminadoM = fit[0] +fit[1];
            }
            else if(fit[0]=='b' && fit[1]=='f'){
                fitPredeterminadoM = fit[0] +fit[1];
            }else{
                this->error= true;
                printf("No se reconoce el fit\n");
            }
        }
        break;
    }
    // Mount
    case 3:
    {
        int cantParametros = raiz->hijos.count();
        QString path = "";
        QString name = "";
        QString parametro;
        for(int i = 0; i < cantParametros; i++){
            parametro=raiz->hijos[i].valor;
         if(parametro == "Path"){
             path = raiz->hijos[i].hijos[0].valor;
         } else if(parametro == "Name"){
             name = raiz->hijos[i].hijos[0].valor;
         }
        }
        if(path != "" && name != ""){
            this->path = path;
            this->name = name;
        }else{
            this->error = true;
            printf("Parametros Obligatorios no encontrados\n");
        }
        break;
    }
    // Unmount
    case 4:
    {
        int cantParametros = raiz->hijos.count();
        QString id = "";
        QString parametro;
        for(int i = 0; i < cantParametros; i++){
            parametro=raiz->hijos[i].valor;
         if(parametro == "ID"){
             id = raiz->hijos[i].hijos[0].valor;
         }
        }
        if(id != ""){
            this->ID = id;
        }else{
            this->error = true;
            printf("Parametros Obligatorios no encontrados\n");
        }
        break;
    }
    // Rep
    case 5:
    {
        int cantParametros = raiz->hijos.count();
        QString path = "";
        QString name = "";
        QString id = "";
        QString parametro;
        for(int i = 0; i < cantParametros; i++){
            parametro=raiz->hijos[i].valor;
         if(parametro == "Path"){
             path = raiz->hijos[i].hijos[0].valor;
         } else if(parametro == "Name"){
             name = raiz->hijos[i].hijos[0].valor;
         } else if(parametro == "ID"){
             id = raiz->hijos[i].hijos[0].valor;
         }
        }
        if(path != "" && name != "" && id != ""){
            this->path = path;
            this->name = name.toLower();
            this->ID = id;
        }else{
            this->error = true;
            printf("Parametros Obligatorios no encontrados\n");
        }
        break;
    }
    // MkFS
    case 6:
    {
        int cantidadHijos = raiz->hijos.count();
        string type = "";
        string id = "";
        for(int i = 0; i < cantidadHijos; i++){
            string parametro = raiz->hijos[i].valor.toStdString();
            if(parametro == "Type"){
                type = raiz->hijos[i].hijos[0].valor.toShort();
            }else if(parametro == "ID"){
                id = raiz->hijos[i].hijos[0].valor.toShort();
            }
        }
        if(id!=""){
            this->ID = QString(id.c_str());
        }else{
            this->error = true;
            cout << "Parametros obligatorios incompletos!" << endl;
        }
        if(type != ""){
            this->typePredeterminado = QString(type.c_str());
        }else{
            this->typePredeterminado = "full";
        }
        break;
    }
    //Login
    case 7:
    {
        int cantidadHijos = raiz->hijos.count();
        string user;
        string pass;
        string id;
        for(int i = 0; i < cantidadHijos; i++){
            string parametro = raiz->hijos[i].valor.toStdString();
            if(parametro == "Usr"){
                user = raiz->hijos[i].hijos[0].valor.toStdString();
            }else if(parametro == "Pwd"){
                pass = raiz->hijos[i].hijos[0].valor.toStdString();
            }else if(parametro == "ID"){
                id = raiz->hijos[i].hijos[0].valor.toStdString();
            }
        }

        if(id != "" && pass != "" && user != ""){
            this->user = user;
            this->pass = pass;
            this->ID = QString(id.c_str());
        }else{
            this->error = true;
            cout << "Parametros Obligatorios faltantes" << endl;
        }
        break;
    }
    //MkUsr
    case 8:
    {
        int cantidadHijos = raiz->hijos.count();
        string user = "";
        string pass = "";
        string grupo = "";
        string parametro = "";
        for(int i = 0; i < cantidadHijos; i++){
            parametro = raiz->hijos[i].valor.toStdString();
            if(parametro == "Usr"){
                user = raiz->hijos[i].hijos[0].valor.toStdString();
            }else if( parametro == "Pwd"){
                pass = raiz->hijos[i].hijos[0].valor.toStdString();
            }else if(parametro == "Grp"){
                grupo = raiz->hijos[i].hijos[0].valor.toStdString();
            }
        }

        if(user != "" && pass != "" && grupo != ""){
            this->user = user;
            this->pass = pass;
            this->grupo = grupo;
        }else{
            cout << "Debe ingresar los parametros necersarios" << endl;
            this->error = true;
        }
        break;
    }
    // Chmod
    case 9:
    {
        int cantidadHijos = raiz->hijos.count();
        string path = "";
        int ugo = -1;
        bool recursivo = false;
        string parametro ="";
        for( int i = 0; i < cantidadHijos; i++){
            parametro = raiz->hijos[i].valor.toStdString();
            if(parametro == "Path"){
                path = raiz->hijos[i].hijos[0].valor.toStdString();
            }else if(parametro == "Ugo"){
                ugo = raiz->hijos[i].hijos[0].valor.toInt();
            }else if(parametro == "R"){
                recursivo = true;
            }
        }

        if(path != "" && ugo > -1){
            this->path = QString(path.c_str());
            this->ugo = ugo;
            this->recursivo = recursivo;
        }else{
            cout << "Falta de parametros obligatorios" << endl;
            this->error = true;
        }
        break;
    }
    // MkFile
    case 10:
    {
        int cantHijos = raiz->hijos.count();
        string path = "";
        bool carpetaPadre = false;
        int size = 0;
        string cont = "";
        string parametro = "";

        for(int i = 0; i < cantHijos; i++){
            parametro = raiz->hijos[i].valor.toStdString();
            if(parametro == "Path"){
                path = raiz->hijos[i].hijos[0].valor.toStdString();
            }else if(parametro == "Size"){
                size = raiz->hijos[i].hijos[0].valor.toInt();
            }else if(parametro == "Cont"){
                cont = raiz->hijos[i].hijos[0].valor.toStdString();
            }else if(parametro == "P"){
                carpetaPadre = true;
            }
        }
        if(path != ""){
            this->path = QString(path.c_str());
            this->size = size;
            this->cont = cont;
            this->carpetaPadre = carpetaPadre;
        }else{
            cout << "Debe indicar la ruta donde se creara el archivo" << endl;
        }

        break;
    }
    // Edit
    case 11:
    {
        int cantHijos = raiz->hijos.count();
        string path = "";
        string cont = "";
        for(int i = 0; i < cantHijos; i++){
            string parametro = raiz->hijos[i].valor.toStdString();
            if(parametro == "Path"){
                path = raiz->hijos[i].hijos[0].valor.toStdString();
            }else if(parametro == "Cont"){
                cont = raiz->hijos[i].hijos[0].valor.toStdString();
            }
        }
        if(path != "" && cont != ""){
            this->path = QString(path.c_str());
            this->cont = cont;
        }else{
            cout << "Parametros obligatorios faltantes" << endl;
            this->error = true;
        }
        break;
    }
    // Ren y find
    case 12:
    {
        int cantHijos = raiz->hijos.count();
        string path = "";
        string name = "";
        for(int i = 0; i < cantHijos; i++){
            string parametro = raiz->hijos[i].valor.toStdString();
            if(parametro == "Path"){
                path = raiz->hijos[i].hijos[0].valor.toStdString();
            }else if(parametro == "Name"){
                name = raiz->hijos[i].hijos[0].valor.toStdString();
            }
        }
        if(name != "" && path != ""){
            this->path = QString(path.c_str());
            this->name = QString(name.c_str());
        }else{
            cout << "Parametros obligatorios faltantes" << endl;
            this->error = true;
        }
        break;
    }
    // MkDir
    case 13:
    {
        int cantHijos = raiz->hijos.count();
        string path = "";
        bool carpetaPadre = false;
        for(int i = 0; i < cantHijos; i++){
            string parametro = raiz->hijos[i].valor.toStdString();
            if(parametro == "Path"){
                path = raiz->hijos[i].hijos[0].valor.toStdString();
            }else if(parametro == "P"){
                carpetaPadre = true;
            }
        }
        if(path != ""){
            this->path = QString(path.c_str());
            this->carpetaPadre = carpetaPadre;
        }else{
            cout << "Parametros obligatorios faltantes" << endl;
            this->error = true;
        }
        break;
    }
    // cp y mv
    case 14:
    {
        int cantHijos = raiz->hijos.count();
        string path = "";
        string dest = "";
        for(int i = 0; i < cantHijos; i++){
            string parametro = raiz->hijos[i].valor.toStdString();
            if(parametro == "Path"){
                path = raiz->hijos[i].hijos[0].valor.toStdString();
            }else if(parametro == "Dest"){
                dest = raiz->hijos[i].hijos[0].valor.toStdString();
            }
        }
        if(dest != "" && path != ""){
            this->path = QString(path.c_str());
            this->dest = dest;
        }else{
            cout << "Parametros obligatorios faltantes" << endl;
            this->error = true;
        }
        break;
    }
    // chown
    case 15:
    {
        int cantHijos = raiz->hijos.count();
        string path = "";
        string user = "";
        bool recursivo = false;
        for(int i = 0; i < cantHijos; i++){
            string parametro = raiz->hijos[i].valor.toStdString();
            if(parametro == "Path"){
                path = raiz->hijos[i].hijos[0].valor.toStdString();
            }else if(parametro == "Usr"){
                user = raiz->hijos[i].hijos[0].valor.toStdString();
            }else if(parametro == "R"){
                recursivo = true;
            }
        }
        if(user != "" && path != ""){
            this->path = QString(path.c_str());
            this->user = user;
            this->recursivo = recursivo;
        }else{
            cout << "Parametros obligatorios faltantes" << endl;
            this->error = true;
        }
        break;
    }
    // chgrp
    case 16:
    {
        int cantHijos = raiz->hijos.count();
        string user = "";
        string grupo = "";
        for(int i = 0; i < cantHijos; i++){
            string parametro = raiz->hijos[i].valor.toStdString();
            if(parametro == "Usr"){
                user = raiz->hijos[i].hijos[0].valor.toStdString();
            }else if(parametro == "Grp"){
                grupo = raiz->hijos[i].hijos[0].valor.toStdString();
            }
        }
        if(user != "" && grupo != ""){
            this->user = user;
            this->grupo = grupo;
        }else{
            cout << "Parametros obligatorios faltantes" << endl;
            this->error = true;
        }
        break;
    }

    }
}



void Interprete::restorePred(){
    this->size = 1;
    this->fitPredeterminadoM = "ff"; // Predeterminado par MkDisk
    this->fitPredeterminadoF = "wf"; //Predeterminado par FDisk
    this->unitPredeterminada = 'm';
    this->path = "";
    this->typePredeterminado = 'p';
    this->deletePredeterminado = "";
    this->name = "";
    this->add = 0;
    this->ID = "";
    this->error = false;
    this->opcion_fdisk = 0;
    this->user = "";
    this->pass = "";
    this->grupo = "";
    this->ugo = 0;
    this->recursivo = false;
    this->carpetaPadre = false;
    this->cont = "";
    this->dest = "";
}

void Interprete::EliminarMount(QString id){
    QList<Mount>::iterator it = montajes->begin();
    bool flag = false;
    while (it != montajes->end()) {
        string a = it->id;
        if(a == id.toStdString()){
            montajes->erase(it);
            flag = true;
            break;
        }else{
            it++;
        }
    }
    if(flag){
        std::cout << "Se desmonto la particion con id: " << id.toStdString() << endl;
    }else{
        std::cout << "No se monto la particion con id: " << id.toStdString() << endl;
    }
}
