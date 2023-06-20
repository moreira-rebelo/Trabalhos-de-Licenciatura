/**
 * PROJETO PROGRAMAÇÃO ORIENTADA A OBJETOS - PRIMEIRA FASE
 * ALUNOS: Allan Aleluia (21990)||Francisco Rebêlo (a16443)||Rui Magalhães (a22300)
 * DOCENTE: Luís Ferreira
 * 
 * Classe PASSE - Para criação dos passes
 **/

using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Reflection.Metadata.Ecma335;
using System.Text;
using System.Threading.Tasks;

namespace BusIPCA
{
    /// <summary>
    /// Classe PASSE - Para criação dos passes
    /// </summary>
    internal class Passe
    {
        /// <summary>
        /// Atributos da classe passe
        /// </summary>
        #region ATRIBUTOS
        private Usuario usuPasse; // atributo que utiliza a classe usuario para acessar os dados do passe
        private int numero;
        private int numeroAluno;
        private string nome;
        private bool status;
        private int ultimoNumero;
        private List<Passe> listaDePasses;

        #endregion


        #region METODOS

        #region PROPRIEDADES

        public Usuario UsuPasse
        {
            get { return usuPasse; }
            set { usuPasse = value; }
        }

        #endregion


        #region CONSTRUTORES
        
        /// <summary>
        /// Construtor por defeito da classe Passe
        /// </summary>
        public Passe()
        {
        usuPasse = new Usuario();
        numero = 0;
        numeroAluno = 0;
        nome = "";
        status = false;
        ultimoNumero = 0;
        listaDePasses = new List<Passe>();
        }
        
       
        #endregion

        #endregion

        #region OUTROS

        /// <summary>
        /// Altera o status do passe para carregado ou não carregado
        /// </summary>
        /// <param name="status"></param>
        /// <returns></returns>
        public bool CarregaPasse (bool status)
        {
            this.status = status;
            return status;
        }

        /// <summary>
        /// Verifica se passe ja existe passando como referencia o possivel numero do passe
        /// </summary>
        /// <param name="numero"></param>
        /// <returns></returns>
        public bool DadosPasse (int numero)
        {
            Passe passe = new Passe();
            if (numero == passe.numero)
            {
                return true;
            }

            return false;
            
        }

        /// <summary>
        /// Método para adicionar um passe criado a lista de passes
        /// </summary>
        /// <param name="passe"></param>
        /// <returns></returns>
        public bool addPasse(Passe passe)
        {
            //verificar se o o passe ja não se encontra na lista
            if (listaDePasses.Contains(passe)) return false; 
            //se não se encontrar na lista, então adicionar a lista.
            listaDePasses.Add(passe);
            return true;
        }
        #endregion
    }
}
