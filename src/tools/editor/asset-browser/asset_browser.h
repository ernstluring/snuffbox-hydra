#pragma once

#include <QWidget>

class QTreeView;
class QSplitter;
class QSettings;
class FlowLayout;

namespace snuffbox
{
  namespace editor
  {
    class AssetBrowserItem;

    /**
    * @brief Used to show Snuffbox's assets within a specific directory, from
    *        which you can open import settings or import them into the engine
    *
    * @author Daniel Konings
    */
    class AssetBrowser : public QWidget
    {

      Q_OBJECT;

    public:

      /**
      * @brief Construct through a Qt parent
      *
      * @param[in] parent The parent widget this widget lives in
      */
      AssetBrowser( QWidget* parent = nullptr);

      /**
      * @brief Saves the splitter state of the asset browser to restore later
      */
      void SaveState(QSettings* settings);

      /**
      * @brief Loads the splitter state of the asset browser, if available
      */
      void LoadState(const QSettings* settings);

    public slots:

      /**
      * @brief Refreshes the asset browser to a new directory
      *
      * @param[in] path The path of the new directory to open
      */
      void Refresh(const QString& path);

      /**
      * @brief Called when a new item is selected
      *
      * @param[in] item The item that was selected
      */
      void OnItemSelect(const AssetBrowserItem* item);

    signals:

      /**
      * @brief Emitted when the current asset browser selection has changed
      *
      * @parma[in] new_item The new item
      */
      void SelectionChanged(const AssetBrowserItem* new_item);

    private:

      QTreeView* tree_; //!< The tree view to navigate the asset tree
      FlowLayout* asset_list_; //!< The asset list layout
      QSplitter* splitter_; //!< The splitter
      const AssetBrowserItem* selected_item_; //!< The currently selected item

      /**
      * @brief The spacing between asset browser items
      */
      static const int kFlowLayoutSpacing_;

      /**
      * @brief The asset browser splitter key
      */
      static const QString kSettingsAssetBrowserKey_;
    };
  }
}