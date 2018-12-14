using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Viewer
{

    public partial class Form2Gui : Form
    {
        private static Form1 mf;
        public Form2Gui(Form1 mainForm)
        {
            mf = mainForm;
            InitializeComponent();
            trackBar1.Scroll += new System.EventHandler(this.trackBar1_Scroll);
        }

        private void trackBar1_Scroll(object sender, System.EventArgs e)
        {
            textBox1.Text = "" + trackBar1.Value;
        }
    }
}


