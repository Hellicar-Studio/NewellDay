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

    public partial class GuiForm : Form
    {
        private static ViewerForm mf;
        public GuiForm(ViewerForm mainForm)
        {
            mf = mainForm;
            InitializeComponent();
            XPosition.ValueChanged += new System.EventHandler(mf.OnParameterChanged);
            YPosition.ValueChanged += new System.EventHandler(mf.OnParameterChanged);
            Width.ValueChanged += new System.EventHandler(mf.OnParameterChanged);
            Height.ValueChanged += new System.EventHandler(mf.OnParameterChanged);

            //trackBar1.Scroll += new System.EventHandler(this.trackBar1_Scroll);
            //trackBar1.Scroll += new System.EventHandler(mf.trackBar1_Scroll);
        }

        private void GuiForm_Load(object sender, EventArgs e)
        {

        }
    }
}


