/**
 * PROJETO PROGRAMAÇÃO ORIENTADA A OBJETOS - PRIMEIRA FASE
 * ALUNOS: Allan Aleluia (21990)||Francisco Rebêlo (a16443)||Rui Magalhães (a22300)
 * DOCENTE: Luís Ferreira
 * 
 * Classe PASSES - Para criação e gerenciamento da lista de passes
 **/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization.Formatters.Binary;
using System.Text;
using System.Threading.Tasks;

namespace BusIPCA
{
    /// <summary>
    ///Classe PASSES - Para criação e gerenciamento da lista de passes 
    /// </summary>
    internal class Passes //Classe dos passes para geração da lista
    {
        #region ATRIBUTOS
        private int idPasse;
        private List<Passes> listaPasses;
        private Passe dadosPasse;
        private int totalPasses;
        private int ultimoIdPasse;
        #endregion

        #region PROPRIEDADES
        private int IdPasse
        {
            get { return idPasse; }
            set { idPasse = value; }
        }

        private Passe DadosPasse
        {
            get { return dadosPasse; }
            set { dadosPasse = value; }
        }
        #endregion

        #region CONSTRUTORES
        /// <summary>
        /// Construtor por defeito de Passes
        /// </summary>
        public Passes()
        {
            listaPasses = new List<Passes>();
            IdPasse = 0;
            dadosPasse = new Passe();
            totalPasses = 0;
            ultimoIdPasse = 0;
        }
        #endregion

        /// <summary>
        /// Método que incrementa a variável idPasse, para que seja sequêncial a criação dos id's
        /// </summary>
        /// <param name="v"></param>
        /// <returns></returns>
        private int GeradorId(int v) 
        {
            ultimoIdPasse++;
            return idPasse;
        }

        /// <summary>
        /// Método que incrementa o total de passes da lista
        /// </summary>
        /// <param name="p"></param>
        /// <returns></returns>
        public int AddPasses(Passe p)
        {
            Passes tmpP = new Passes();

            idPasse = GeradorId(ultimoIdPasse);
            tmpP.IdPasse = idPasse;
            tmpP.DadosPasse = p;

            listaPasses.Add(tmpP);

            totalPasses++;

            return idPasse;
        }

        /// <summary>
        /// Método que procura novos passes para adicionar a lista
        /// </summary>
        /// <param name="nidpasse"></param>
        /// <returns></returns>
        public Passes ProcuraPasse(int nidpasse)
        {
            Passes tmp = new Passes();
            
            foreach (var item in listaPasses)
            {
                if(item.IdPasse == nidpasse)
                {
                    tmp = item;
                    break;
                }
            }

            return tmp;
        }

        /// <summary>
        /// Método para gerar uma lista com os passes através do idpasse, uma vez que a lista não esteja vazia.
        /// </summary>
        /// <returns></returns>
        public bool ListaPasses()
        {
            int tmpIdPasse = 0;
            int tmpNumMec = 0;
            string tmpNome = "";
            string tmpEmail = "";

            Console.WriteLine("************  LISTA DE PASSES  ************");
            if (listaPasses.Count != 0)
            {
                tmpIdPasse = 0;
                tmpNumMec = 0;
                tmpNome = "";
                tmpEmail = "";

                foreach (var item in listaPasses) //procura os itens nos dados do passe para imprimir na tela
                {
                    tmpIdPasse = item.IdPasse;
                    tmpNumMec = item.DadosPasse.UsuPasse.NumMec;
                    tmpNome = item.DadosPasse.UsuPasse.Nome;
                    tmpEmail = item.DadosPasse.UsuPasse.Email;

                    Console.WriteLine("");
                    Console.WriteLine("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*");
                    Console.WriteLine("Id Passe: {0}", tmpIdPasse);
                    Console.WriteLine("Nome: {0}", tmpNome);
                    Console.WriteLine("Número Mecanográfico: {0}", tmpNumMec);
                    Console.WriteLine("E-mail: {0}", tmpEmail);
                    Console.WriteLine("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*");

                }
            }

            Console.WriteLine("");
            return true;

        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="fileName"></param>
        /// <returns></returns>
        public bool SaveAll(string docpasses)
        {
            try
            {
                Stream stream = File.Open(docpasses, FileMode.OpenOrCreate);
                BinaryFormatter bin = new BinaryFormatter();
                bin.Serialize(stream, listaPasses);
                stream.Close();
                return true;
            }
            catch (IOException e)
            {
                throw e;
            }
        }
    }
}
