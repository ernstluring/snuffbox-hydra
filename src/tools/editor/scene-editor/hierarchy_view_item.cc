#include "tools/editor/scene-editor/hierarchy_view_item.h"
#include "tools/editor/scene-editor/hierarchy_view.h"

#include <engine/ecs/entity.h>

namespace snuffbox
{
  namespace editor
  {
    //--------------------------------------------------------------------------
    HierarchyViewItem::HierarchyViewItem(
      engine::Entity* entity, 
      HierarchyView* view) :
      QTreeWidgetItem(view),
      view_(view),
      parent_(nullptr),
      entity_(entity)
    {
      SetDefaults();
    }

    //--------------------------------------------------------------------------
    HierarchyViewItem::HierarchyViewItem(
      engine::Entity* entity,
      HierarchyView* view,
      HierarchyViewItem* parent) :
      QTreeWidgetItem(parent),
      view_(view),
      parent_(parent),
      entity_(entity)
    {
      SetDefaults();
    }

    //--------------------------------------------------------------------------
    void HierarchyViewItem::SetDefaults()
    {
      setFlags(
        flags() |
        Qt::ItemIsEditable | 
        Qt::ItemIsDragEnabled | 
        Qt::ItemIsDropEnabled);

      Update();
    }

    //--------------------------------------------------------------------------
    bool HierarchyViewItem::SetParent(HierarchyViewItem* new_parent)
    {
      HierarchyViewItem* old_parent = parent_;
      parent_ = new_parent;

      return old_parent != parent_;
    }

    //--------------------------------------------------------------------------
    void HierarchyViewItem::Update()
    {
      setText(0, entity_->name().c_str());
      setTextColor(0, entity_->IsActive() ? Qt::white : Qt::gray);
    }

    //--------------------------------------------------------------------------
    engine::Entity* HierarchyViewItem::entity() const
    {
      return entity_;
    }

    //--------------------------------------------------------------------------
    HierarchyViewItem::~HierarchyViewItem()
    {
      view_->UnmapItem(this);
    }
  }
}