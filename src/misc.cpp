double ErrorCuadraticoMedio(Vector &b, Vector &bcalculado)
{
    double ecm=0;
    size_t cantElem;

    cantElem = b.size();

    for (int i=0; i<cantElem; ++i)
    {
        ecm += (b[i]-bcalculado[i])^2;
    }

    ecm = ecm/(n^2)

    return ecm;
}