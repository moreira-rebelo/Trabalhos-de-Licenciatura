/**
 * PROJETO PROGRAMAÇÃO ORIENTADA A OBJETOS - PRIMEIRA FASE
 * ALUNOS: Allan Aleluia (21990)||Francisco Rebêlo (a16443)||Rui Magalhães (a22300)
 * DOCENTE: Luís Ferreira
 * 
 * Classe LOCAL - Para criação das paragens
 **/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BusIPCA
{
    /// <summary>
    /// Classe LOCAL - Para criação das paragens
    /// </summary>
    internal class Local
    {
        #region ATRIBUTOS
        private string nome;
        private int codigo;
        List<Local> ListaDeParagens = new List<Local>();

        #endregion

        #region METODOS

        #region PROPRIEDADES

        public string Nome
        {
            get { return nome; }
            set { this.nome = value; }
        }

        public int Codigo
        {
            get { return codigo; }
            set { this.codigo = value; }
        }


        #region CONSTRUTORES

        /// <summary>
        /// Construtor que recebe por parâmetro os valores de Local
        /// </summary>
        /// <param name="nome"></param>
        /// <param name="id"></param>
        public Local(string nome, int id)
        {
            this.nome = nome;
            this.codigo = id;
        }
        #endregion

        /// <summary>
        /// Construtor por defeito
        /// </summary>
        public Local()
        {
            this.nome = "";
            this.codigo = 0;
        }

        #endregion
        #endregion
    }
}
