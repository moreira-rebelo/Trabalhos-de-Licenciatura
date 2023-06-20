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

        public int Numero
        {
            get { return numero;}
            set { this.numero = value;}
        }

        public int NumeroAluno
        {
            get { return numeroAluno; }
            set { this.numeroAluno = value; }
        }

        public string Nome
        {
            get { return nome; }
            set { this.nome = value; }
        }

        public bool Status
        {
            get { return status;}
            set { this.status = value; }
        }

        public int UltimoNumero
        {
            get { return ultimoNumero; }
            set { this.ultimoNumero = value; }
        }

        public List<Passe> ListaDePasses
        {
            get { return listaDePasses; }
            set { listaDePasses = value; }
        }


        #endregion


        #region CONSTRUTORES
        
        /// <summary>
        /// Construtor por defeito da classe Passe
        /// </summary>
        public Passe()
        {
        this.usuPasse = new Usuario();
        this.numero = 0;
        this.numeroAluno = 0;
        this.nome = "";
        this.status = false;
        this.ultimoNumero = 0;
        this.listaDePasses = new List<Passe>();
        }
        
        /// <summary>
        /// Cobstrutor que toma por parâmetro as várias propriedades
        /// </summary>
        /// <param name="usuPasse"></param>
        /// <param name="numero"></param>
        /// <param name="numeroAluno"></param>
        /// <param name="nome"></param>
        /// <param name="status"></param>
        /// <param name="ultimoNumero"></param>
        public Passe(int usuPasse, int numero, int numeroAluno, string nome, bool status, int ultimoNumero)
        {
            this.usuPasse = new Usuario();
            this.numero = 0;
            this.numeroAluno = 0;
            this.nome = "";
            this.status = false;
            this.ultimoNumero = 0;
            this.listaDePasses = new List<Passe>();
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
