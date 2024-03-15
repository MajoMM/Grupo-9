//
simple nodo
{
	parameters:
	    volatile double intervalo @unit(s)=default(1s);
	
    gates:
        input entrada;
        output salida;
        
}

network txrx
{
    submodules:
        tx: nodo {
            parameters:
                 @display("i=block/source");
                 
        }
        rx: nodo {
            parameters:
                 @display("i=block/sink");
        }
    connections:
        tx.salida -->  { }--> rx.entrada;
        tx.entrada <-- { } <-- rx.salida;

}
