/***************** <Tweek heading BEGIN do not edit this line> ****************
 * Tweek
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 ***************** <Tweek heading END do not edit this line> *****************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.tweek.gui;

import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import javax.swing.*;
import javax.swing.event.*;
import org.vrjuggler.tweek.beans.BeanPreferences;


/**
 * An extension of javax.swing.JDialog that acts as a container for any
 * preference editors that Beans may have.  The Beans themselves are
 * responsible for providing their own preferences editors--this class just
 * collects them into a single dialog box and presents it to the GUI user.
 *
 * @since 1.0
 */
public class BeanPrefsDialog extends JDialog
{
   public BeanPrefsDialog (java.awt.Frame owner, String title)
   {
      super(owner, title);
      init();

      this.setModal(true);
      this.setLocationRelativeTo(owner);
   }

   /**
    * Default constructor.  This is here mainly to placate JBuilder, but it
    * can be used instead of the constructor that specifies an owner and
    * title.
    */
   public BeanPrefsDialog ()
   {
      init();
   }

   /**
    * Adds the given Bean to the collection of Beans with preferences editors.
    * When the dialog box is opened, the editor associated with the Bean will
    * be included in the GUI.
    */
   public void addPrefsBean (BeanPreferences bean)
   {
      mPrefsBeans.add(bean);
      mTabbedViewPane.add(new JScrollPane(bean.getEditor()),
                          bean.getEditorName());
      bean.initGUI();
      mTabbedViewPane.revalidate();
      DefaultListModel model = (DefaultListModel) mListViewBeans.getModel();
      model.addElement(bean.getEditorName());
   }

   /**
    * Returns the number of Beans with editable preferences.
    */
   public int getPrefsBeanCount()
   {
      return mPrefsBeans.size();
   }

   /**
    * Displays this dialog box.  Call this when the user wants to edit Bean
    * preferences.
    */
   public void display ()
   {
      this.pack();
      this.setVisible(true);
   }

   public int getStatus ()
   {
      return status;
   }

   public static final int OK_OPTION     = JOptionPane.OK_OPTION;
   public static final int CANCEL_OPTION = JOptionPane.CANCEL_OPTION;
   public static final int CLOSED_OPTION = JOptionPane.CLOSED_OPTION;

   protected void processWindowEvent (WindowEvent e)
   {
      super.processWindowEvent(e);

      if ( e.getID() == WindowEvent.WINDOW_CLOSING )
      {
         status = CLOSED_OPTION;
      }
   }

   private void init ()
   {
      enableEvents(AWTEvent.WINDOW_EVENT_MASK);

      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }
   }

   /**
    * JBuilder's initializer method.  The code contained within is entirely
    * generated by JBuilder.
    */
   private void jbInit() throws Exception
   {
      mCancelButton.setMnemonic('C');
      mCancelButton.setText("Cancel");
      mCancelButton.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            cancelButtonClicked(e);
         }
      });
      mSaveButton.setMnemonic('S');
      mSaveButton.setText("Save");
      mSaveButton.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            saveButtonClicked(e);
         }
      });
      mOkButton.setMnemonic('O');
      mOkButton.setSelected(true);
      mOkButton.setText("OK");
      mOkButton.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            okButtonClicked(e);
         }
      });
      mListViewButton.setEnabled(false);
      mListViewButton.setToolTipText("");
      mListViewButton.setMnemonic('L');
      mListViewButton.setText("List View");
      mListViewButton.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            listViewButtonClicked(e);
         }
      });
      mTabbedViewButton.setMnemonic('T');
      mTabbedViewButton.setSelected(true);
      mTabbedViewButton.setText("Tabbed View");
      mTabbedViewButton.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            tabbedViewButtonClicked(e);
         }
      });
      mViewPanel.setMinimumSize(new Dimension(300, 200));
      mViewPanel.setPreferredSize(new Dimension(300, 200));
      mViewPanel.setLayout(mViewPanelLayout);
      mListViewBeans.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
      mListViewBeans.addListSelectionListener(new javax.swing.event.ListSelectionListener()
      {
         public void valueChanged(ListSelectionEvent e)
         {
            prefsBeanListValueChanged(e);
         }
      });
      mListViewSplitPane.setDividerSize(5);
      this.getContentPane().add(mButtonPanel, BorderLayout.SOUTH);
      mButtonPanel.add(mOkButton, null);
      mButtonPanel.add(mSaveButton, null);
      mButtonPanel.add(mCancelButton, null);
      this.getContentPane().add(mViewButtonPanel,  BorderLayout.NORTH);
      mViewButtonPanel.add(mTabbedViewButton, null);
      mViewButtonPanel.add(mListViewButton, null);
      this.getContentPane().add(mViewPanel, BorderLayout.CENTER);
      mViewPanel.add(mTabbedViewPane, BorderLayout.CENTER);
      mViewButtonGroup.add(mTabbedViewButton);
      mViewButtonGroup.add(mListViewButton);
      mListViewSplitPane.add(mListViewBeansPane, JSplitPane.LEFT);
      mListViewSplitPane.add(mListViewEditorPane, JSplitPane.RIGHT);
      mListViewBeansPane.getViewport().add(mListViewBeans, null);
   }

   /**
    * Switch to the tabbed view of the editors.
    */
   private void tabbedViewButtonClicked (ActionEvent e)
   {
      mViewPanel.remove(mListViewSplitPane);
      mViewPanel.add(mTabbedViewPane, BorderLayout.CENTER);
      mViewPanel.revalidate();
      mViewPanel.repaint();
   }

   /**
    * Switch to the split pane + list view of the editors.
    */
   private void listViewButtonClicked (ActionEvent e)
   {
      mViewPanel.remove(mTabbedViewPane);
      mViewPanel.add(mListViewSplitPane, BorderLayout.CENTER);
      mViewPanel.revalidate();
      mViewPanel.repaint();
   }

   private void prefsBeanListValueChanged (ListSelectionEvent e)
   {
      BeanPreferences bean =
         (BeanPreferences) mPrefsBeans.elementAt(e.getFirstIndex());
      mListViewEditorPane.getViewport().add(bean.getEditor());
      mListViewEditorPane.revalidate();
   }

   /**
    * Checks all the Bean preference editor panels to determine if they are in
    * a state that allows any changes to be committed.
    */
   private boolean canCommit ()
   {
      java.util.Iterator i = mPrefsBeans.iterator();
      boolean can_commit = true;

      while ( i.hasNext() )
      {
         if ( ! ((BeanPreferences) i.next()).canCommit() )
         {
            can_commit = false;
            break;
         }
      }

      return can_commit;
   }

   /**
    * Commits changes on all known Bean preference editors.
    */
   private void commit ()
   {
      java.util.Iterator i = mPrefsBeans.iterator();

      while ( i.hasNext() )
      {
         BeanPreferences bean = (BeanPreferences) i.next();

         try
         {
            bean.save();
         }
         catch (java.io.IOException io_ex)
         {
            JOptionPane.showMessageDialog(null,
                                          "Could not save preferences for " +
                                             bean.getEditorName() + ": " +
                                             io_ex.getMessage(),
                                          "I/O Exception on Save",
                                          JOptionPane.ERROR_MESSAGE);
         }
      }
   }

   private void okButtonClicked (ActionEvent e)
   {
      if ( canCommit() )
      {
         status = OK_OPTION;
         commit();
         this.setVisible(false);
      }
   }

   private void saveButtonClicked(ActionEvent e)
   {
      if ( canCommit() )
      {
         commit();
      }
   }

   private void cancelButtonClicked (ActionEvent e)
   {
      status = CANCEL_OPTION;
      setVisible(false);
   }

   private int status;

   private Vector mPrefsBeans = new Vector();

   private JPanel mButtonPanel = new JPanel();
   private JButton mCancelButton = new JButton();
   private JButton mSaveButton = new JButton();
   private JButton mOkButton = new JButton();
   private JPanel mViewButtonPanel = new JPanel();
   private ButtonGroup mViewButtonGroup = new ButtonGroup();
   private JRadioButton mListViewButton = new JRadioButton();
   private JRadioButton mTabbedViewButton = new JRadioButton();
   private JSplitPane mListViewSplitPane = new JSplitPane();
   private JScrollPane mListViewEditorPane = new JScrollPane();
   private JPanel mViewPanel = new JPanel();
   private JTabbedPane mTabbedViewPane = new JTabbedPane();
   private BorderLayout mViewPanelLayout = new BorderLayout();
   private JScrollPane mListViewBeansPane = new JScrollPane();
   private JList mListViewBeans = new JList(new DefaultListModel());
}
