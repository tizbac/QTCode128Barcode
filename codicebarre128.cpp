/*
 * 
Copyright (c) 2014 Tiziano Bacocco

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE. */


#include "codicebarre128.h"
#include <stdio.h>


QList<QString> CodiceBarre128::barcode_cache_list;
QMap<QString,QPixmap> CodiceBarre128::barcode_cache_map;
CodiceBarre128::CodiceBarre128(QString str, int sizemult) //La stringa deve essere con le lettere maiuscole
{
    m_str = str;
    m_image = NULL;
    if ( barcode_cache_map.contains(str) )
    {
        return;
    }
    
    initChars();
    m_image = new QImage(CodiceBarre128::computeWidth(str,sizemult),BARCODE_HEIGHT,QImage::Format_RGB888);
    m_pos = 0;
    for ( int i = 0; i < 15; i++ )
    {
        drawEmptyVerticalLineAtPos();
    }
    
    m_sizemult = sizemult;
    int checksum = 103;
    encodeChar(caratteri_by_valore[103]);
    for ( int i = 0; i < str.length(); i++ )
    {
	CarattereCodiceBarre128 c = caratteri_by_nome[QString(str[i])];
	encodeChar(c);
	checksum += c.m_valore*(i+1);
    }
    checksum %= 103;
    encodeChar(caratteri_by_valore[checksum]);
    encodeChar(caratteri_by_valore[106]);
    for ( int i = 0; i < 15; i++ )
    {
        drawEmptyVerticalLineAtPos();
    }
    
    
    barcode_cache_list.push_back(str);
    barcode_cache_map.insert(str,getPixmap());
    
    if ( barcode_cache_list.size() > BARCODE_CACHE_SIZE )//Ruota la cache
    {
        barcode_cache_map.remove(barcode_cache_list.first());
        barcode_cache_list.removeFirst();
    }
}
void CodiceBarre128::initChars()
{
    initChar(0," ","11011001100");
    initChar(1,"!","11001101100");
    initChar(2,"\"","11001100110");
    initChar(3,"#","10010011000");
    initChar(4,"$","10010001100");
    initChar(5,"%","10001001100");
    initChar(6,"&","10011001000");
    initChar(7,"'","10011000100");
    initChar(8,"(","10001100100");
    initChar(9,")","11001001000");
    initChar(10,"*","11001000100");
    initChar(11,"+","11000100100");
    initChar(12,"0","10110011100");
    initChar(13,"-","10011011100");
    initChar(14,".","10011001110");
    initChar(15,"/","10111001100");
    initChar(16,"0","10011101100");
    initChar(17,"1","10011100110");
    initChar(18,"2","11001110010");
    initChar(19,"3","11001011100");
    initChar(20,"4","11001001110");
    initChar(21,"5","11011100100");
    initChar(22,"6","11001110100");
    initChar(23,"7","11101101110");
    initChar(24,"8","11101001100");
    initChar(25,"9","11100101100");
    initChar(26,":","11100100110");
    initChar(27,";","11101100100");
    initChar(28,"<","11100110100");
    initChar(29,"=","11100110010");
    initChar(30,">","11011011000");
    initChar(31,"?","11011000110");
    initChar(32,"@","11000110110");
    initChar(33,"A","10100011000");
    initChar(34,"B","10001011000");
    initChar(35,"C","10001000110");
    initChar(36,"D","10110001000");
    initChar(37,"E","10001101000");
    initChar(38,"F","10001100010");
    initChar(39,"G","11010001000");
    initChar(40,"H","11000101000");
    initChar(41,"I","11000100010");
    initChar(42,"J","10110111000");
    initChar(43,"K","10110001110");
    initChar(44,"L","10001101110");
    initChar(45,"M","10111011000");
    initChar(46,"N","10111000110");
    initChar(47,"O","10001110110");
    initChar(48,"P","11101110110");
    initChar(49,"Q","11010001110");
    initChar(50,"R","11000101110");
    initChar(51,"S","11011101000");
    initChar(52,"T","11011100010");
    initChar(53,"U","11011101110");
    initChar(54,"V","11101011000");
    initChar(55,"W","11101000110");
    initChar(56,"X","11100010110");
    initChar(57,"Y","11101101000");
    initChar(58,"Z","11101100010");
    initChar(59,"[","11100011010");
    initChar(60,"\\","11101111010");
    initChar(61,"]","11001000010");
    initChar(62,"^","11110001010");
    initChar(63,"_","10100110000");
    initChar(64,"NUL","10100001100");
    initChar(65,"SOH","10010110000");
    initChar(66,"STX","10010000110");
    initChar(67,"ETX","10000101100");
    initChar(68,"EOT","10000100110");
    initChar(69,"ENQ","10110010000");
    initChar(70,"ACK","10110000100");
    initChar(71,"BEL","10011010000");
    initChar(72,"BS","10011000010");
    initChar(73,"HT","10000110100");
    initChar(74,"LF","10000110010");
    initChar(75,"VT","11000010010");
    initChar(76,"FF","11001010000");
    initChar(77,"CR","11110111010");
    initChar(78,"SO","11000010100");
    initChar(79,"SI","10001111010");
    initChar(80,"DLE","10100111100");
    initChar(81,"DC1","10010111100");
    initChar(82,"DC2","10010011110");
    initChar(83,"DC3","10111100100");
    initChar(84,"DC4","10011110100");
    initChar(85,"NAK","10011110010");
    initChar(86,"SYN","11110100100");
    initChar(87,"ETB","11110010100");
    initChar(88,"CAN","11110010010");
    initChar(89,"EM","11011011110");
    initChar(90,"SUB","11011110110");
    initChar(91,"ESC","11110110110");
    initChar(92,"FS","10101111000");
    initChar(93,"GS","10100011110");
    initChar(94,"RS","10001011110");
    initChar(95,"US","10111101000");
    initChar(96,"FNC 3","10111100010");
    initChar(97,"FNC 2","11110101000");
    initChar(98,"Shift B","11110100010");
    initChar(99,"Code C","10111011110");
    initChar(100,"Code B","10111101110");
    initChar(101,"FNC 4","11101011110");
    initChar(102,"FNC 1","11110101110");
    initChar(103,"STARTA","11010000100");
    initChar(104,"Start Code B","11010010000");
    initChar(105,"Start Code C","11010011100");
    initChar(106,"STOP","1100011101011");
}



void CodiceBarre128::initChar(int value, QString character, QString pattern)
{
    CarattereCodiceBarre128 c(value,character,pattern);
    caratteri_by_nome.insert(character,c);
    caratteri_by_valore.insert(value,c);
    
}

CodiceBarre128::~CodiceBarre128()
{
    if ( m_image )
        delete m_image;
}

void CodiceBarre128::encodeChar(CarattereCodiceBarre128 c)
{
    QString pattern = c.m_pattern;
    for ( int x = 0; x < pattern.length(); x++ )
    {
	for ( int k = 0; k < m_sizemult; k++ )
	{
	      if ( pattern[x] == QChar('1') )
	      {
		  drawBlackVerticalLineAtPos();
	      }else{
		  drawEmptyVerticalLineAtPos();
	      }
	}
      
    }
}

void CodiceBarre128::drawBlackVerticalLineAtPos()
{
    if ( m_pos >= m_image->width() )
    {
	printf("%d > %d\n",m_pos,m_image->width());
	abort();
    }
    for ( int y = 0; y < m_image->height(); y++ )
    {
	m_image->setPixel(m_pos,y,qRgb(0,0,0));
    }
    m_pos++;
}

void CodiceBarre128::drawEmptyVerticalLineAtPos()
{
    for ( int y = 0; y < m_image->height(); y++ )
    {
	m_image->setPixel(m_pos,y,qRgb(255,255,255));
    }
    m_pos++;
}

QPixmap CodiceBarre128::getPixmap()
{
    if ( barcode_cache_map.contains(m_str) )
        return barcode_cache_map.value(m_str);
    return QPixmap::fromImage(*m_image);
}


CarattereCodiceBarre128::CarattereCodiceBarre128(int value, QString nome, QString pattern)
{
    m_valore = value; m_nome = nome; m_pattern = pattern;
}

CarattereCodiceBarre128::CarattereCodiceBarre128(const CarattereCodiceBarre128& other)
{
    m_valore = other.m_valore;
    m_nome = other.m_nome;
    m_pattern = other.m_pattern;
}

CarattereCodiceBarre128& CarattereCodiceBarre128::operator=(const CarattereCodiceBarre128& other)
{
    m_valore = other.m_valore;
    m_nome = other.m_nome;
    m_pattern = other.m_pattern;
    return *this;
}
CarattereCodiceBarre128::CarattereCodiceBarre128()
{
    m_nome = "NONVALIDO";
    m_valore = -1;
    m_pattern = "1111111";
}

int CodiceBarre128::computeWidth(QString str, int sizemult)
{
    return (str.length()*11+13+11+11)*sizemult+30;
}



#include "codicebarre128.moc"
