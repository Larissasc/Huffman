#include "huffman.h"
#include <iomanip>

huffman::huffman()
{
    loading = 0;
}

//A compactação é feita  lendo a string do endereço(olhar a função fillList)
void huffman::encodes(QString inputAddress, QString name)
{
    readInput(inputAddress);

    fillList();

    setStringTree();

    buildTree();

    setNewCodes();

    QBitArray coded;

    for(int i = 0; i < arquivo.size(); ++i)
    {
        QByteArray newcode;

        search(arquivo.mid(i, 1), &lista[0], newcode);

        int lastIndex = coded.size();

//aumenta o array para caber mais bits
        coded.resize(coded.size() + newcode.size());

//coloca os bits no espaço criado
        for(int j = 0; j < newcode.size(); ++j)
            if(newcode[j] == '1')
                coded.setBit(lastIndex + j);
    }

    trash = 8 - (coded.size() % 8);

    float bef = arquivo.size();
    cout << "Tamanho antes: "<< arquivo.size() << " Bytes" << endl << endl;

    arquivo = bitsToBytes(coded);

    float aft2 = arquivo.size();

    writeHFF(name);

    float aft = arquivo.size();
    float rate;

    rate = 100*(1 - aft2/bef);
    cout << "Porcentagem de compressao sem arvore: " << rate << "%"<< endl << endl;

    rate = 100*(1 - aft/bef);
    cout << "Porcentagem de compressao com arvore: " << rate << "%"<< endl << endl;

    lista.clear();
    stringTree.clear();
    arquivo.clear();
}

void huffman::decodes(QString HFFAddress, QString name)
{
    lista.clear();
    arquivo.clear();
    stringTree.clear();

    readHFF(HFFAddress);

    getinfo();

    readTree();

    int i = 0;

    QBitArray code = bytesToBits(arquivo);
    QByteArray decoded;

 //anda na arvore e quando acha uma folha coloca o caracter no arquivo
    while(i < code.size() - trash)
    {
        node *  tmp = &lista[0];

        while(!isleaf(tmp))
        {
            if(code[i])
                tmp = tmp->right;
            else
                tmp = tmp->left;

            ++i;

            float r = 100*(float(i)/float(code.size() - trash - 1));
            decoded += tmp->character;
        }
    }

    arquivo = decoded;
    writeOutput(name);


}

//Coloca a frequencia na lista
void huffman::fillList()
{
    int frequency[256];

    for(int i = 0; i < 256; i++)
        frequency[i] = arquivo.count(i);

    for(int i = 0; i < 256; ++i)
    {
        if(frequency[i])
        {
            node * tmp = new node;
            tmp->frequency = frequency[i];

            if(i > 127)
                tmp->character.append(-i + 127);
            else
                tmp->character.append(i);

            lista << * tmp;
        }
    }

    sortList();
}

//faz uma string com caracter, frequencia: 1byte, 3 bytes
void huffman::setStringTree()
{
    node tmp;

    for(int i = 0; i < lista.size(); ++i)
    {
        tmp = lista.at(i);

        if(int(tmp.character[0]) == 0)
            stringTree.resize(stringTree.size() + 1);
        else
            stringTree.append(tmp.character);

        QString frequency;
        frequency = frequency.setNum(tmp.frequency, 2).rightJustified(16, '0');

        bool ok;

        int firstByte;

        firstByte = frequency.mid(0, 8).toInt(&ok, 2);

        if(firstByte > 127)
            firstByte = -firstByte + 127;
        stringTree.append(firstByte);

        int secondByte;

        secondByte = frequency.mid(8, 8).toInt(&ok, 2);
        if(secondByte > 127)
            secondByte = -secondByte + 127;
        stringTree.append(secondByte);

        int thirdByte;

        thirdByte = frequency.mid(16, 8).toInt(&ok, 2);
        if(thirdByte > 127)
            thirdByte = -thirdByte + 127;
        stringTree.append(thirdByte);
    }
}

void huffman::buildTree()
{
    while(lista.size() > 1)
    {
        node tmp;

        node * left = new node;
        left->frequency = lista[0].frequency;
        left->left  = lista[0].left;
        left->character = lista[0].character;
        left->right = lista[0].right;

        lista.removeFirst();
        tmp.left = left;

        node * right = new node;
        right->frequency = lista[0].frequency;
        right->left  = lista[0].left;
        right->character = lista[0].character;
        right->right = lista[0].right;

        lista.removeFirst();
        tmp.right = right;

        tmp.frequency = left->frequency + right->frequency;

        lista << tmp;

        sortList();
    }
}

bool huffman::isleaf(node * tmp)
{
    return(!tmp->left);
}

void huffman::sortList()
{
    bool swapped = true;

    if(lista.size() == 1)
        return;

    while(swapped)
    {
        swapped = false;
        for(int i = 1; i < lista.size(); ++i)
            if(lista[i].frequency < lista[i - 1].frequency)
            {
                lista.swap(i, i-1);
                swapped = true;
            }
    }
}

void huffman::setNewCodes()
{
    QByteArray nulo;
    setNewCodes(&lista[0], nulo);
}

//faz busca em profundidade e coloca os codigos nas folhas
void huffman::setNewCodes(node * current, QByteArray key)
{
    if(isleaf(current))
    {
        current->newBits = key;
    }
    else
    {
        QByteArray keyLeft = key + '0';
        setNewCodes(current->left, keyLeft);

        QByteArray keyRight = key + '1';
        setNewCodes(current->right, keyRight);
    }
}

//recebe um caracter e quando acha o nó com um caracter igual, salva seu codigo em newCode
void huffman::search(QByteArray code, node * begin, QByteArray &newCode)
{
    if(!isleaf(begin))
    {
        search(code, begin->left, newCode);

        search(code, begin->right, newCode);
    }
    else if(begin->character == code)
    {
        newCode.append(begin->newBits);
        return;
    }
}

//pega caracter, frequencia da lista e salva na litsa, remove da arvore
void huffman::getinfo()
{
    bool ok;

    trash = arquivo.left(1).toInt(&ok, 10);

    int sizeStringTree;

    if(int(arquivo[2]) < 0)
        sizeStringTree = -arquivo[2] + 127;
    else
        sizeStringTree = arquivo[2];

    if(int(arquivo[1]) < 0)
        sizeStringTree += (-arquivo[1] + 127)*256;
    else
        sizeStringTree += 256*arquivo[1];

    stringTree = arquivo.mid(3, sizeStringTree);

    arquivo.remove(0, 3 + sizeStringTree);
}

//pega carac, freq da lista e constroi a arvore
void huffman::readTree()
{
    int i = 0;

    while(i < stringTree.size())
    {
        int letter = stringTree[i];

        long int frequency;

        if(int(stringTree[i + 3]) < 0)
            frequency = -stringTree[i + 3] + 127;
        else
            frequency = stringTree[i + 3];

        if(int(stringTree[i + 2]) < 0)
            frequency += (-stringTree[i + 2] + 127)*256;
        else
            frequency += 256*stringTree[i + 2];

        if(int(stringTree[i + 1]) < 0)
            frequency += (-stringTree[i + 1] + 127)*65536;
        else
            frequency += 65536*stringTree[i + 1];

        node * tmp = new node;
        tmp->frequency = frequency;
        tmp->character += letter;

        lista << * tmp;

        i += 4;
    }

    sortList();
    stringTree.clear();
    buildTree();
    setNewCodes();
}

void huffman::readInput(QString inputAddress)
{
    QFile Input(inputAddress);

    if(Input.open (QIODevice::ReadOnly))
    {
        cout << "File was read" << endl << endl;
        arquivo = Input.readAll();
    }

    Input.close();
}

void huffman::writeOutput(QString name)
{
    QString outputFilename = "C:\\Users\\Lari\\Desktop\\Larissa\\" + name;
    QFile file(outputFilename);

    if (file.open (QIODevice::WriteOnly)){
        cout << "File was written" << endl << endl;
        file.write(arquivo);
    }
}

void huffman::readHFF(QString HFFAddress)
{
    QFile Input(HFFAddress);

    if(Input.open (QIODevice::ReadOnly)) {
        cout << "HFF file was read" << endl << endl;
        arquivo = Input.readAll();
    }

    Input.close();
}

void huffman::writeHFF(QString name)
{
    QByteArray hff;

    QString stringTrash;
    stringTrash = stringTrash.setNum(trash);
    hff.append(stringTrash);

    QString stringSizeTree;
    stringSizeTree = stringSizeTree.setNum(stringTree.size(), 2).rightJustified(16, '0');

    bool error;
    stringSizeTree += stringSizeTree.mid(0, 8).toInt(&error, 2);
    stringSizeTree += stringSizeTree.mid(8, 8).toInt(&error, 2);
    stringSizeTree = stringSizeTree.right(2);

    hff.append(stringSizeTree);

    hff.append(stringTree);

    cout << "Tamanho do HFF sem arvore: " << arquivo.size() << " Bytes" << endl <<endl;

    arquivo = hff + arquivo;

    cout << "Tamanho do HFF com arvore: " << arquivo.size() << " Bytes" << endl <<endl;

    cout << "==============" << name.toStdString() << endl;

    QString HFFAddress = "C:\\Users\\Lari\\Desktop\\Larissa\\" + name;

    QFile file(HFFAddress);
    cout << file.fileName().toStdString() << endl;

    if ( file.open (QIODevice::WriteOnly)) {
        cout << "HFF file was written" << endl << endl;
        file.write(arquivo);

    }
    file.close();
}

QByteArray huffman::bitsToBytes(QBitArray bits)
{
    QByteArray bytes;

    if(bits.count() % 8 != 0)
        bytes.resize((bits.count()/8) + 1);
    else
        bytes.resize(bits.count()/8);
    bytes.fill(0);

    for(int b = 0; b < bits.count(); ++b)
        bytes[b/8] = ( bytes.at(b/8) | ((bits[b]?1:0)<<(b%8)));
    return bytes;
}

QBitArray huffman::bytesToBits(QByteArray bytes)
{
    QBitArray bits(bytes.count()*8);

    for(int i = 0; i < bytes.count(); ++i)
        for(int b=0; b < 8; ++b)
            bits.setBit((i*8)+b, bytes.at(i)&(1<<b));
    return bits;
}
