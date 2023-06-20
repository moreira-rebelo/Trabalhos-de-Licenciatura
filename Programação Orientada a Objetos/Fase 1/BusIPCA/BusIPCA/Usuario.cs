/**
 * PROJETO PROGRAMAÇÃO ORIENTADA A OBJETOS - PRIMEIRA FASE
 * ALUNOS: Allan Aleluia (21990)||Francisco Rebêlo (a16443)||Rui Magalhães (a22300)
 * DOCENTE: Luís Ferreira
 * 
 * Classe USUÁRIO - Para criação de um usuário herdando atributos da classe pessoa
 **/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BusIPCA
{
    /// <summary>
    /// Classe USUÁRIO - Para criação de um usuário herdando atributos da classe pessoa
    /// </summary>
    internal class Usuario : Pessoa //a classe usuario herda a classe pessoa
    {
        #region ATRIBUTOS

        /// <summary>
        /// Atribui um passe a uma pessoa
        /// </summary>
        private Passe atribuirPasse;

        #endregion

        #region METODOS

        #region PROPRIEDADES

        public Passe AtribuirPasse
        {
            get { return atribuirPasse; }
            set { this.atribuirPasse = value; }
        }

        #endregion

        #region CONSTRUTORES

        /// <summary>
       /// Construtor que recebe por parâmetro os valores a inerir em Usuário
       /// </summary>
       /// <param name="idPessoa"></param>
       /// <param name="nome"></param>
       /// <param name="telemovel"></param>
       /// <param name="email"></param>
        public Usuario(int idPessoa, string nome, int telemovel, string email) : base (idPessoa, nome, telemovel, email) // "base" usado para informar quais atributos seram herdados da classe pessoa
        {
            this.atribuirPasse = new Passe();
        }

       /// <summary>
       /// Construtor que geral que não recebe nada por parâmetro
       /// </summary>
        public Usuario() : base()
        {
            this.atribuirPasse = new Passe();
        }

        #endregion
        #endregion  


    }
}
