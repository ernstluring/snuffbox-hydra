#pragma once

#include "engine/ecs/component.h"
#include <scripting/script_class.h>

#include <foundation/serialization/serializable.h>
#include <foundation/containers/vector.h>
#include <foundation/memory/memory.h>
#include <foundation/auxiliary/logger.h>
#include <foundation/containers/uuid.h>

#include <cstddef>

namespace snuffbox
{
  namespace engine
  {
    class Scene;

    /**
    * @brief An entity class to hold a multitude of components that are updated
    *        through the Scene
    *
    * @author Daniel Konings
    */
    SCRIPT_CLASS() class Entity : 
      public scripting::ScriptClass, 
      public foundation::ISerializable
    {

      friend Scene;

    public:

      /**
      * @brief Used to store serialized, typed, component data
      *
      * @remarks Because we don't know exactly what component type we have
      *          yet before deserialization, we store the component type first
      *          and then deserialize again to retrieve the actual data.
      *
      * @author Daniel Konings
      */
      struct SerializedComponent
      {
        Components type; //!< The type of the stored component
        IComponent* data; //!< The data of the stored component
      };

      SCRIPT_NAME(Entity);

      /**
      * @brief Construct and add a TransformComponent
      */
      Entity();

      /**
      * @brief Construct an entity within a specific scene
      *
      * @param[in] scene The scene to add this entity to
      * @param[in] internal Is this entity for internal use?
      */
      Entity(Scene* scene, bool internal = false);

      SCRIPT_CONSTRUCTOR(Entity);

#ifndef SNUFF_NSCRIPTING
      /**
      * @brief Creates a script object for a component and adds some short-hand
      *        fields to retrieve values of the parent entity
      *
      * The fields are:
      * - entity: The entity parent
      * - transform: The transform of the entity, but this is only added
      *              if the type of the component to add is not a transform
      *              already
      *
      * @param[in] e The entity to 
      * @param[in] c The component to create a script object for
      * @param[in] type The component type, if it's a transform component
      *                 the "transform" field is not added
      */
      static scripting::ScriptObjectHandle CreateScriptComponent(
          Entity* e,
          IComponent* c,
          Components type);
#endif

      /**
      * @brief Adds a typed component and returns it
      *
      * @tparam T The component type
      *
      * @return The created component, or nullptr if it couldn't be added
      */
      template <typename T>
      T* AddComponent();

      /**
      * @brief Adds a component by ID
      *
      * @param[in] id The component ID
      *
      * @see Entity::CreateComponentByID
      */
      SCRIPT_FUNC(custom) IComponent* AddComponent(Components id);

      /**
      * @brief Adds a component by ID at a specific index
      *
      * @remarks If the index is greater than the size, it will be clamped to
      *          the size and insert at the end, you can also use -1 for this
      *
      * @param[in] id The component ID
      * @param[in] index The index to insert at
      *
      * @see Entity::CreateComponentById
      */
      SCRIPT_FUNC(custom) IComponent* AddComponentAt(Components id, int index);
      
      /**
      * @brief Removes the first found typed component
      *
      * @tparam T The component type
      */
      template <typename T>
      void RemoveComponent();

      /**
      * @brief Removes the first found typed component by ID
      *
      * @param[in] id The ID of the component to remove
      */
      SCRIPT_FUNC() void RemoveComponent(Components id);

      /**
      * @brief Removes a component at a specific index by ID
      *
      * @param[in] id The ID of the component to remove
      * @param[in] index The index of the component to remove
      */
      SCRIPT_FUNC() void RemoveComponentAt(Components id, int index);

      /**
      * @brief Removes a component by reference
      *
      * @remarks This function loops through every component list in the entity
      *          and thus might be slower than other removal options
      *
      * @param[in] component The component to remove
      */
      void RemoveComponentByRef(IComponent* component);

      /**
      * @brief Removes all components of a certain type
      *
      * @tparam T The component type
      */
      template <typename T>
      void RemoveComponents();

      /**
      * @brief Removes all components of a certain type by ID
      *
      * @param[in] id The ID of the components to remove
      */
      SCRIPT_FUNC() void RemoveComponents(Components id);

      /**
      * @brief Checks if a typed component exists within this entity
      *
      * @tparam T The component type
      *
      * @return Do we have more than 0 components of this type?
      */
      template <typename T>
      bool HasComponent() const;

      /**
      * @brief Checks if a typed component exists within this entity by ID
      *
      * @param[in] id The ID of the component to check
      *
      * @return Do we have more than 0 components of this type?
      */
      SCRIPT_FUNC() bool HasComponent(Components id) const;

      /**
      * @brief Retrieves the first typed component found within this entity
      *
      * @tparam T The component type
      *
      * @return The found component, or nullptr if it doesn't exist
      */
      template <typename T>
      T* GetComponent() const;

      /**
      * @brief Retrieves the first typed component found within 
      *        this entity by ID
      *
      * @param[in] id The ID of the component to retrieve
      *
      * @return The found component, or nullptr if it doesn't exist
      */
      SCRIPT_FUNC(custom) IComponent* GetComponent(Components id) const;

      /**
      * @brief Retrieves all components of a certain component type
      *
      * @tparam T The component type
      *
      * @return The list of found components
      */
      template <typename T>
      foundation::Vector<T*> GetComponents() const;

      /**
      * @brief Retrieves all components of a certain component type by ID
      *
      * @param[in] id The ID of the component to retrieve
      *
      * @return The list of found components
      */
      SCRIPT_FUNC(custom) foundation::Vector<IComponent*> GetComponents(
        Components id) const;

      /**
      * @brief Destroys this entity and all its components
      */
      SCRIPT_FUNC() void Destroy();

      /**
      * @brief Sets the name of this entity
      *
      * @param[in] name The name to set
      */
      SCRIPT_FUNC() void set_name(const foundation::String& name);

      /**
      * @return The name of this entity
      */
      SCRIPT_FUNC() const foundation::String& name() const;

      /**
      * @brief Sets if this entity is active
      *
      * @param[in] active Is this entity active?
      */
      SCRIPT_FUNC() void set_active(bool active);

      /**
      * @return Is this entity active?
      *
      * @remarks This function does not propagate the parent tree and simply
      *          returns whether this specific entity is active or not
      */
      SCRIPT_FUNC() bool active() const;

      /**
      * @return Is this entity active?
      *
      * @remarks This function propagates the entire parent tree, if any
      *          of the upper-level parents are inactive, this entity is treated
      *          as inactive as well
      */
      SCRIPT_FUNC() bool IsActive() const;

      /**
      * @return The total number of components on this entity, including its
      *         transform component
      */
      SCRIPT_FUNC() int ComponentCount() const;

      /**
      * @return Is this an internal entity?
      */
      bool is_internal() const;

      /**
      * @return The transform component of this entity
      */
      SCRIPT_FUNC() TransformComponent* transform() const;

    protected:

      /**
      * @brief Adds a component to the entity by ID
      *
      * @param[in] id The ID of the component to add
      * @param[in] index The index to insert the component at
      *
      * @return The created component
      */
      IComponent* AddComponentInternal(Components id, int index = -1);

      /**
      * @brief Checks if a component type is actually derived from its
      *        respective component base
      *
      * This function will static_assert if the type is incorrect
      *
      * @tparam T The type to check
      */
      template <typename T>
      static void TypeCheck();

      /**
      * @brief Checks if a component type is a valid type to use
      *
      * @param[in] id The ID to check
      *
      * @remarks This method asserts if the component type is not supported
      */
      static void IDCheck(Components id);

      /**
      * @brief Short-hand for the underlying structure of the components array
      */
      using ComponentArray = 
        foundation::Vector<foundation::UniquePtr<IComponent>>;

      /**
      * @brief Retrieves the component array of a certain component type
      *
      * @param[in] id The component type
      *
      * @return The underlying component array
      */
      const ComponentArray& GetComponentArray(Components id) const;

      /**
      * @brief A type definition for the functions to create components
      *
      * @see IComponent::CreateComponent
      */
      using ComponentCreateFunc = IComponent*(*)(Entity*);

      /**
      * @brief A short-hand to easily reach the component creation functions
      */
      using ComponentCreateArray = foundation::Vector<ComponentCreateFunc>;

      /**
      * @brief Retrieves the list of component creation functions by ID
      *
      * The return array will be a list of function pointers that have a
      * 1 on 1 mapping to their respective component IDs
      *
      * @remarks The first time this function is called, all assignments to
      *          the creation array will be done
      *
      * @see Entity::AssignComponentCreator
      *
      * @return The function pointer array
      */
      static const ComponentCreateArray& ComponentCreators();

      /**
      * @brief Assigns a component creation function by component ID, to the
      *        list of component creation functions
      *
      * @remarks This function recurses until Components::kCount is found.
      *          This function is a compile-time unrolled loop.
      *
      * @tparam C The current ID
      *
      * @param[in] arr The list to add the component creation function to
      */
      template <Components C>
      static void AssignComponentCreator(ComponentCreateArray& arr);

      /**
      * @brief Used to create a component by ID, using the creation functions
      *        assigned in Entity::AssignComponentCreator
      *
      * @param[in] id The ID of the component to create
      *
      * @return The base component pointer to the created component
      */
      IComponent* CreateComponentByID(Components id);

    public:


      /**
      * @brief Called when the scene starts or when the scene is already
      *        running and the entity is created
      */
      void Start();

      /**
      * @brief Updates this entity and its components
      *
      * @remarks The entity is only updated if the entity itself is active
      *
      * @param[in] dt The current delta-time of the application
      */
      void Update(float dt);

      /**
      * @brief This method can be overridden to do custom C++-sided behavior
      *        after inheritance
      *
      * @param[in] dt The current delta-time of the application
      *
      * @see Entity::Update
      */
      virtual void OnUpdate(float dt);

      /**
      * @return The scene this entity was spawned in
      */
      Scene* scene() const;

      /**
      * @return The UUID of this entity
      */
      const foundation::UUID& uuid() const;

      /**
      * @brief Sets a new UUID for this entity
      *
      * @param[in] uuid The UUID to set
      */
      void set_uuid(const foundation::UUID& uuid);
      
      /**
      * @return The sort index of the entity, used in editor
      */
      int sort_index() const;

      /**
      * @brief Sets the sort index of this entity, so that we can re-order
      *        entities within the hierarchy view of the editor
      *
      * @param[in] idx The new sort index
      */
      void set_sort_index(int idx);

      /**
      * @see ISerializable::Serialize
      */
      void Serialize(foundation::SaveArchive& archive) const override;

      /**
      * @see ISerializable::Deserialize
      */
      void Deserialize(foundation::LoadArchive& archive) override;

      /**
      * @brief Destructs the entity and removes it from the scene
      */
      ~Entity();

    private:

      /**
      * @brief The list of components, of which each component can be
      *        contained multiple times
      */
      ComponentArray components_[static_cast<size_t>(Components::kCount)];

      foundation::String name_; //!< The name of this entity
      bool destroyed_; //!< Has this entity been destroyed yet?
      bool active_; //!< Is this entity active?

      /**
      * @brief Is this an internal entity?
      *
      * Internal entities can be used by the editor for functionalities like
      * the editor camera. This avoids writing duplicate code and makes the
      * entity pipeline accessible without exposing it to the end-user.
      *
      * @remarks Internal entities are not serialized, nor destroyed when a
      *          scene is cleared. It's up to the user to destroy these entities
      */
      bool is_internal_;

      Scene* scene_; //!< The scene this entity was spawned in
      foundation::UUID uuid_; //!< The UUID of this entity

      int sort_index_; //!< The sorting index of the entity, used in editor

      static const char* kDefaultName_; //!< The default name for entities
    };

    //--------------------------------------------------------------------------
    template <typename T>
    inline T* Entity::AddComponent()
    {
      TypeCheck<T>();
      return static_cast<T*>(AddComponent(T::type_id));
    }

    //--------------------------------------------------------------------------
    template <typename T>
    inline void Entity::RemoveComponent()
    {
      TypeCheck<T>();
      RemoveComponent(T::type_id);
    }

    //--------------------------------------------------------------------------
    template <typename T>
    inline void Entity::RemoveComponents()
    {
      TypeCheck<T>();
      RemoveComponents(T::type_id);
    }

    //--------------------------------------------------------------------------
    template <typename T>
    inline bool Entity::HasComponent() const
    {
      TypeCheck<T>();
      return HasComponent(T::type_id);
    }

    //--------------------------------------------------------------------------
    template <typename T>
    inline T* Entity::GetComponent() const
    {
      TypeCheck<T>();
      return static_cast<T*>(GetComponent(T::type_id));
    }

    //--------------------------------------------------------------------------
    template <typename T>
    inline foundation::Vector<T*> Entity::GetComponents() const
    {
      TypeCheck<T>();

      foundation::Vector<T*> result;
      foundation::Vector<IComponent*> comps = GetComponents(T::type_id);

      result.resize(comps.size());

      for (size_t i = 0; i < comps.size(); ++i)
      {
        result.at(i) = static_cast<T*>(comps.at(i));
      }

      return result;
    }

    //--------------------------------------------------------------------------
    template <typename T>
    void Entity::TypeCheck()
    {
      const Components id = T::type_id;

      static_assert(eastl::is_base_of<ComponentBase<T, id>, T>::value == true,
        "Attempted to add an invalid component");
    }

    //--------------------------------------------------------------------------
    template <Components C>
    inline void Entity::AssignComponentCreator(ComponentCreateArray& arr)
    {
      const size_t id = static_cast<size_t>(C);
      arr.at(id) = &IComponent::CreateComponent<C>;
      
      const size_t next = id + 1;
      AssignComponentCreator<static_cast<Components>(next)>(arr);
    }

    //--------------------------------------------------------------------------
    template <>
    inline void Entity::AssignComponentCreator<Components::kCount>(
      ComponentCreateArray& arr)
    {

    }
  }
}