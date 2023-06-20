/**
 * PROJETO PROGRAMAÇÃO ORIENTADA A OBJETOS - PRIMEIRA FASE
 * ALUNOS: Allan Aleluia (21990)||Francisco Rebêlo (a16443)||Rui Magalhães (a22300)
 * DOCENTE: Luís Ferreira
 * 
 * O projeto tende a trazer praticidade ao utilizar os autocarros que dão acesso ao IPCA
 **/

using System.Security.Cryptography;

namespace BusIPCA
{
    internal class Program
    {
        static void Main(string[] args)
        {
            //Cria um utilizador
            Usuario u1 = new Usuario(21990, "Allan", 999999999, "email@xpto");

            //Faz a atribuição de um passe a um utilizador
            u1.AtribuirPasse = new Passe(1, 1, 21990, "Allan", false, 0);
        }
    }
}
