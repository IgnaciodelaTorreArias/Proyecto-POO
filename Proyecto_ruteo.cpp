#include <iostream>
#include <string>
#include <vector>
#include <clocale>
#include <stdexcept>
#include <algorithm>

using namespace std;

namespace myProject {
    void get(unsigned short &var) noexcept(false) {
        short variable = 0;
        for (int i = 0; i < 100; i++) {
            if (cin.eof()) {
                throw runtime_error((string)"Rompiste el bucle");
            }
            if (!(cin >> variable).good()) {
                cout << "La entrada debe de ser de tipo numérico" << endl;
            } else {
                if (variable < 1) {
                    cout << "La entrada debe ser un numero positivo mayor a 0" << endl;
                } else {
                    var = variable;
                    break;
                }
            }
            cin.clear();
            cin.ignore();
        }
    }
}

template<class T>
class collection{
protected:
    vector<T> datos;
public:
    [[nodiscard]]int search(unsigned short element){
        if (datos.empty()){
            throw out_of_range((string)"No se tienen datos en los que buscar");
        }
        int menor = 0;
        int mayor = datos.size();
        int pivot = (mayor - menor) / 2;
        while(menor <= mayor && pivot < datos.size()){
            if(datos[pivot].get() == element){
                return pivot;
            }
            if(element < datos[pivot].get()){
                mayor = pivot-1;
            }else{
                menor = pivot+1;
            }
            pivot = (mayor - menor) / 2 + menor;
        }
        return -(pivot);
    }
    virtual collection& operator+ (unsigned short num){return *this;}
    void print(){
        for(const T &val : datos){
            cout << val.get() << endl;
        }
    }
};

//Function de distribution externa para permitir pruebas
void distribute(vector<unsigned short> &rutas, vector<unsigned short> &paquetes){
    if(paquetes.empty()){
        throw out_of_range((string)"No tienes paquetes que mandar");
    }
    while (paquetes[paquetes.size() - 1] == 0){  // nos desasemos de los valores basura que aparecen magicamente
        paquetes.pop_back();
    }
    while (paquetes[paquetes.size() - 1] > rutas[rutas.size() - 1]){  // Se retiran los paquetes que no pueden ser entregados
        cout << "El paquete " << paquetes.size() - 1 << " supera la maxima capacidad de la ruta mayor, El peso del paquete era: " << paquetes[paquetes.size()-1] << endl;
        paquetes.pop_back();
    }
    float pesoRutas = 0;
    for(const unsigned short &path:rutas){
        pesoRutas += (float)path;  //Sumatoria
    }
    int pesoPaquetes = 0;
    for(const unsigned short &paquete:paquetes){
        pesoPaquetes += paquete;  //Sumatoria
    }
    pesoRutas = (float)pesoPaquetes / pesoRutas > 1? 1:(float)pesoPaquetes / pesoRutas; // Se saca la relación paquetes-rutas, si la relación supera la capacidad de las rutas se disminuye a una relación 1-1
    pesoPaquetes = 0; //se recicla la variable
    for(int i=0; i < rutas.size() ;i++) { // Se recorren las rutas para asignarles paquetes
        int real = 0;  // Peso que realmente pasa
        cout << "--------------------------------" << endl;
        cout << "Para la la ruta: " << i << endl;
        cout << "Con peso: " << rutas[i] << endl;
        cout << "Su peso ideal a pasar es: " << (float)rutas[i] * pesoRutas << endl;
        cout << "Los paquetes que pasan por esta ruta son: " << endl;
        while(!paquetes.empty()){ //Se distribuyen los paquetes
            if((float)(real + paquetes[0]) < ((float)rutas[i] * pesoRutas + ((float)rutas[i] * pesoRutas) / 2)){  //Si sumando el paquete no se pasa el límite ideal entonces el paquete se envía por la ruta (hay un margen de error)
                real += paquetes[0];
                cout << "El paquete: " << pesoPaquetes++ << "\tde peso: " << paquetes[0] << endl;
                paquetes.erase(paquetes.begin()); //Ya que tanto paquetes como rutas se ordenan de menor a mayor se van llenando las rutas más pequeñas con los paquetes más pequeños
            }
            else{
                break;  //Si sumando el paquete se sobrepasa el límite se dejan de revisar paquetes para esta ruta y se pasa a la siguiente
            }
        }
        cout << "El peso que realmente paso fue: " << real << endl;
    }
    cout << "\n\nLos paquetes que por alguna razor no se enviaron son: ";
    for (unsigned short &error : paquetes) {
        cout << error << ", ";
    }
}

class ARP{
//Classes with access to protected and private atributes
friend class Router;
friend class Network;
friend void distribute(vector<unsigned short> &rutas, vector<unsigned short> &paquetes);
private:
    vector<unsigned short> datos;
    unsigned short destiny_ID; //Can't be const because it causas an error when vector tries to use a copy constructor. However, it'll act as a const.
    //Disable a constructor, so that only friend classes are allowed to use this class.
    explicit ARP(unsigned short ID):destiny_ID(ID){}
public:
    ~ARP(){
        datos.clear();
    }
    [[nodiscard]] inline unsigned short get() const{
        return destiny_ID;
    }
    void print(){
        int i = 0;
        for(const unsigned short &value : datos){
            cout << "Ruta: " << i++ << " | Peso: " << value << endl;
        }
    }
    void send(vector<unsigned short> &paquetes) noexcept(false){
        if(!datos.empty()){
            distribute(datos, paquetes);
            return;
        }
        throw out_of_range((string)"No hay rutas por las cuales mandar los paquetes");
    }
    void del_Route(){
        cout << "Numero de ruta | Peso de la ruta";
        print();
        unsigned short temp = datos.size();
        while(temp > datos.size()-1) {
            cout << "Cual es el numero de ruta que quieres eliminar? ";
            myProject::get(temp);
        }
        datos.erase(datos.begin() + temp);
    }
    ARP& operator+(unsigned short num){
        if(datos.empty()){
            datos.emplace_back(num);
            return *this;
        }
        int aproximate = abs(search(num));
        if(num < datos[aproximate]){
            datos.insert(datos.begin() + aproximate, num);
            return *this;
        }
        if (aproximate + 1 < datos.size()) {
            datos.insert(datos.begin() + aproximate + 1, num);
            return *this;
        }
        datos.emplace_back(num);
        return *this;
    }
    [[nodiscard]]int search(unsigned short element){
        if (datos.empty()){
            throw out_of_range((string)"No se tienen datos en los que buscar");
        }
        int menor = 0;
        int mayor = (int)datos.size();
        int pivot = (mayor - menor) / 2;
        while(menor <= mayor && pivot < datos.size()){
            if(datos[pivot] == element){
                return pivot;
            }
            if(element < datos[pivot]){
                mayor = pivot-1;
            }else{
                menor = pivot+1;
            }
            pivot = (mayor - menor) / 2 + menor;
        }
        return -(pivot);
    }
};

class Router: public collection<ARP>{
friend class Network;
private:
    static unsigned short numero_de_routers; //static to count instances
    unsigned short ID; //unique identifier
    Router():ID(++numero_de_routers){} //assignment of ID an increment of instances count
public:
    ~Router(){
        datos.clear();
    }
    [[nodiscard]] inline unsigned short get() const{
        return ID;
    }
    int buscar_ARP(unsigned short destiny) noexcept(false){
        int pos = search(destiny);
        if(datos[abs(pos)].get() == destiny) {
            return pos;
        }
        throw invalid_argument((string)"El router " + to_string(ID) + " no se conecta con el router " + to_string(destiny));
    }
    void send(unsigned short destiny, vector<unsigned short> &paquetes){
        datos[buscar_ARP(destiny)].send(paquetes);
    }
    void showRoutes(unsigned short destiny){
        cout << "Las rutas del router " << ID << " al router " << destiny << " son: " << endl;
        datos[buscar_ARP(destiny)].print();
    }
    void del_ARP(unsigned short destiny){
        datos.erase(datos.begin() + buscar_ARP(destiny));
    }
    void del_Route(unsigned short destiny){
        datos[buscar_ARP(destiny)].del_Route();
    }
    void add_Route(unsigned short destiny, unsigned short weight) noexcept(false){
        datos[buscar_ARP(destiny)] + weight;
    }
    Router& operator+(unsigned short num) noexcept(false) override{
        if(datos.empty()){
            datos.emplace_back(ARP(num));
            return *this;
        }
        int aproximate = abs(search(num));
        if(datos[aproximate].get() == num){
            throw invalid_argument((string)"El router " + to_string(ID) + " ya esta conectado con el router " + to_string(num));
        }
        if(num < datos[aproximate].get()){
            datos.insert(datos.begin() + aproximate, ARP(num));
            return *this;
        }
        if (aproximate + 1 < datos.size()) {
            datos.insert(datos.begin() + abs(aproximate) + 1, ARP(num));
            return *this;
        }
        datos.emplace_back(ARP(num));
        return *this;
    }
};
unsigned short Router::numero_de_routers = 0;

class Network: private collection<Router>{
public:
    ~Network(){
        datos.clear();
    }
    int buscar_Router(unsigned short ID) noexcept(false){
        int pos = search(ID);
        if (datos[abs(pos)].get() == ID){
            return pos;
        }
        throw length_error((string)"El router " + to_string(ID) + " no se encontró en la red");
    }
    int buscar_ARP(unsigned short origin, unsigned short destiny, int pos_R=-1) noexcept(false){
        if (pos_R == -1){
            pos_R = buscar_Router(origin);
        }
        return datos[pos_R].buscar_ARP(destiny);
    }
    void showRouters(){
        cout << "Los IDs de los routers en la Red son: ";
        print();
    }
    void showARP(unsigned short ID){
        int pos = buscar_Router(ID);
        cout << "Los IDs de los router a los que se conecta el router " << ID << " son: " << endl;
        datos[pos].print();
    }
    void showRoutes(unsigned short origin, unsigned short destiny){
        int pos_R = buscar_Router(origin);
        datos[pos_R].showRoutes(destiny);
    }
    void send(unsigned short origin, unsigned short destiny, vector<unsigned short> &paquetes) noexcept(false){
        int pos = buscar_Router(origin);
        buscar_Router(destiny);
        datos[pos].send(destiny, paquetes);
    }
    void connect(unsigned short ID_Router, unsigned short Destiny_ID, bool bidirectional) noexcept(false){
        if(ID_Router == Destiny_ID) {
            throw invalid_argument((string)"No puedes crear lazos entre los routers");
        }
        int pos_R = buscar_Router(ID_Router);
        int pos_A = buscar_Router(Destiny_ID);
        datos[pos_R] + Destiny_ID;
        if (bidirectional) {
            datos[pos_A] + ID_Router; //Swap of IDs
        }
    }
    void add_Route(unsigned short origin, unsigned short destiny, unsigned short weight) noexcept(false){
        buscar_Router(destiny);  //confirms that destiny router exists before trying to add the route.
        datos[buscar_Router(origin)].add_Route(destiny, weight);
    }
    void del_Router(unsigned short ID) noexcept(false){
        datos.erase(datos.begin() + buscar_Router(ID));
    }
    void del_ARP(unsigned short ID_Router, unsigned short destiny_ID) noexcept(false){
        datos[buscar_Router(ID_Router)].del_ARP(destiny_ID);
    }
    void del_Route(unsigned short ID_Router, unsigned short destiny_ID) noexcept(false){
        datos[buscar_Router(ID_Router)].del_Route(destiny_ID);
    }
    Network& operator+(unsigned short num) override{
        for(;num > 0;num--){
            datos.emplace_back(Router());
        }
        return *this;
    }
};

void getBoth(unsigned short &origin, unsigned short &destiny){
    cout << "Cual es el router origen? ";
    myProject::get(origin);
    cin.clear();
    cout << "Cual es el router destino? ";
    myProject::get(destiny);
}

int main(){
    setlocale(LC_ALL,"spanish");
    auto *Red = new Network();
    unsigned short opc = 0, origin, destiny;
    while (true){
        cout << "Que quires hacer?\n"
                "1) Mandar paquetes\n"
                "2) Agregar nuevos routers\n"
                "3) Conectar dos routers\n"
                "4) Agregar una ruta entre dos routers\n"
                "5) Listar los routers en la red\n"
                "6) Listar las conexiones de un router\n"
                "7) Listar las rutas de un router a otro\n"
                "8) Eliminar un router\n"
                "9) Eliminar la conexión entre dos routers\n"
                "10) Eliminar una ruta entre dos routers\n"
                "11) Salir\n";
        cin >> opc;
        try {
            switch (opc) {
                case 1: {
                    vector<unsigned short> paquetes;
                    cout << "Cuantos paquetes quieres mandar? ";
                    unsigned short val;
                    myProject::get(val);
                    paquetes.resize(val);
                    for (int i = 0; i < val; i++) {
                        cout << "Dame el valor del paquete en la posición " << i << ": ";
                        myProject::get(destiny);
                        paquetes.insert(paquetes.begin() + i, destiny);
                    }
                    cout << "Cual es ID del router origen? ";
                    myProject::get(origin);
                    cout << "Cual es ID del router destino? ";
                    myProject::get(destiny);
                    Red->send(origin, destiny, paquetes);
                    break;
                }
                case 2:{
                    cout << "Cuantos routers quieres agregar? ";
                    myProject::get(origin);
                    (*Red) + origin;
                    break;
                }
                case 3:{
                    bool bidirectional;
                    getBoth(origin, destiny);
                    cout << "Quieres que la ruta sea bidireccional {1) si | 0) no}? ";  //clear instructions, please don't break
                    cin >> bidirectional;
                    Red->connect(origin, destiny, bidirectional);
                    break;
                }
                case 4:{
                    unsigned short weight;
                    getBoth(origin, destiny);
                    cout << "Cual es el peso de la ruta? ";
                    myProject::get(weight);
                    Red->add_Route(origin, destiny, weight);
                    break;
                }
                case 5:{
                    Red->showRouters();
                    break;
                }
                case 6:{
                    cout << "De que router quieres ver sus conexiones? ";
                    myProject::get(origin);
                    Red->showARP(origin);
                    break;
                }
                case 7:{
                    getBoth(origin, destiny);
                    Red->showRoutes(origin, destiny);
                    break;
                }
                case 8:{
                    cout << "Que router quieres eliminar? ";
                    myProject::get(origin);
                    Red->del_Router(origin);
                    break;
                }
                case 9:{
                    getBoth(origin, destiny);
                    Red->del_ARP(origin, destiny);
                    break;
                }
                case 10:{
                    getBoth(origin, destiny);
                    Red->del_Route(origin, destiny);
                    break;
                }
                case 11:
                    cout << "----------Adios----------";
                    return 0;
                default:
                    cout << "Opción no valida";
                    break;
            }
        }
        catch (logic_error &result) {
            cout << result.what() << endl;
        }
        catch (runtime_error &result){
            cout << result.what() << endl;
        }
    }
}