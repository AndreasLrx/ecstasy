/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "ECSTASY", "index.html", [
    [ "ECSTASY (Entity Component System Toward Architecture Saving Years)", "index.html", "index" ],
    [ "Building", "md_doc_2_building.html", [
      [ "Prerequisites", "md_doc_2_building.html#prerequisites", null ],
      [ "Build Steps", "md_doc_2_building.html#build-steps", [
        [ "IDEs", "md_doc_2_building.html#ides", null ]
      ] ],
      [ "Using as a CMake package", "md_doc_2_building.html#using-as-a-cmake-package", null ],
      [ "CMake options", "md_doc_2_building.html#CMakeOptions", null ]
    ] ],
    [ "Glossary", "md_doc_2_glossary.html", [
      [ "Entity", "md_doc_2_glossary.html#entity", null ],
      [ "Component", "md_doc_2_glossary.html#component", null ],
      [ "Storage", "md_doc_2_glossary.html#storage", null ],
      [ "System", "md_doc_2_glossary.html#system", null ],
      [ "Resource", "md_doc_2_glossary.html#resource", null ],
      [ "Registry", "md_doc_2_glossary.html#registry", null ],
      [ "Query", "md_doc_2_glossary.html#query", [
        [ "Queryable", "md_doc_2_glossary.html#queryable", null ],
        [ "Batch Query", "md_doc_2_glossary.html#batch-query", null ],
        [ "Modifier", "md_doc_2_glossary.html#modifier", null ],
        [ "Condition", "md_doc_2_glossary.html#condition", null ]
      ] ],
      [ "Thread", "md_doc_2_glossary.html#thread", [
        [ "Lockable", "md_doc_2_glossary.html#lockable", null ]
      ] ],
      [ "Serialization", "md_doc_2_glossary.html#serialization", [
        [ "Serializer actions", "md_doc_2_glossary.html#serializer-actions", [
          [ "Export", "md_doc_2_glossary.html#export", null ],
          [ "Import", "md_doc_2_glossary.html#import", null ]
        ] ],
        [ "Variable actions", "md_doc_2_glossary.html#variable-actions", [
          [ "Serialize: Save", "md_doc_2_glossary.html#serialize-save", null ],
          [ "Deserialize: Update", "md_doc_2_glossary.html#deserialize-update", null ],
          [ "Deserialize: Load", "md_doc_2_glossary.html#deserialize-load", null ]
        ] ]
      ] ]
    ] ],
    [ "Tutorial", "md_doc_2_tutorial.html", [
      [ "Getting Started", "md_doc_2_tutorial.html#GettingStarted", null ],
      [ "Using entities", "md_doc_2_tutorial.html#using-entities", [
        [ "Creating entities", "md_doc_2_tutorial.html#creating-entities", [
          [ "Using type resolution", "md_doc_2_tutorial.html#using-type-resolution", null ]
        ] ],
        [ "Manage entity component", "md_doc_2_tutorial.html#manage-entity-component", null ],
        [ "Delete entities", "md_doc_2_tutorial.html#delete-entities", null ]
      ] ],
      [ "Implement a custom storage", "md_doc_2_tutorial.html#CustomStorageTutorial", null ],
      [ "Making registry queries", "md_doc_2_tutorial.html#QueryTutorial", [
        [ "Select ... Where ...", "md_doc_2_tutorial.html#SelectWhereTutorial", null ],
        [ "Using modifiers", "md_doc_2_tutorial.html#ModifiersTutorial", null ],
        [ "Using conditions", "md_doc_2_tutorial.html#ConditionsTutorial", null ]
      ] ],
      [ "Using systems", "md_doc_2_tutorial.html#using-systems", null ],
      [ "Using resources", "md_doc_2_tutorial.html#using-resources", null ],
      [ "Ensuring Thread Safety", "md_doc_2_tutorial.html#EnsuringThreadSafety", null ],
      [ "Using cross platform RTTI", "md_doc_2_tutorial.html#using-cross-platform-rtti", [
        [ "Why use custom RTTI implementation", "md_doc_2_tutorial.html#why-use-custom-rtti-implementation", null ],
        [ "How to use it", "md_doc_2_tutorial.html#how-to-use-it", [
          [ "How is it stored", "md_doc_2_tutorial.html#how-is-it-stored", null ],
          [ "Registering types", "md_doc_2_tutorial.html#registering-types", null ],
          [ "Querying types", "md_doc_2_tutorial.html#querying-types", null ]
        ] ]
      ] ],
      [ "Serializing your entities", "md_doc_2_tutorial.html#serializing-your-entities", [
        [ "Using a serializer", "md_doc_2_tutorial.html#using-a-serializer", [
          [ "Common types", "md_doc_2_tutorial.html#common-types", null ],
          [ "Custom types", "md_doc_2_tutorial.html#custom-types", null ],
          [ "Working with Entities", "md_doc_2_tutorial.html#working-with-entities", null ]
        ] ]
      ] ]
    ] ],
    [ "Technical Implementations", "md_doc_2_technical_implementation.html", [
      [ "Entities Management", "md_doc_2_technical_implementation.html#entities-management", null ],
      [ "Components Management (MapStorage)", "md_doc_2_technical_implementation.html#components-management-mapstorage", null ],
      [ "Query", "md_doc_2_technical_implementation.html#query-1", [
        [ "Queryable and BitSet", "md_doc_2_technical_implementation.html#queryable-and-bitset", null ],
        [ "Registry basic query", "md_doc_2_technical_implementation.html#registry-basic-query", null ],
        [ "Select where", "md_doc_2_technical_implementation.html#select-where", null ],
        [ "Modifiers", "md_doc_2_technical_implementation.html#modifiers", null ],
        [ "Conditions", "md_doc_2_technical_implementation.html#conditions", null ],
        [ "AutoLock", "md_doc_2_technical_implementation.html#autolock", null ],
        [ "Resolution Order", "md_doc_2_technical_implementation.html#resolution-order", null ]
      ] ]
    ] ],
    [ "Events integration", "md_doc_2integrations_2events.html", [
      [ "Description", "md_doc_2integrations_2events.html#description", [
        [ "How to get feedback", "md_doc_2integrations_2events.html#how-to-get-feedback", null ],
        [ "Event flow", "md_doc_2integrations_2events.html#event-flow", null ]
      ] ],
      [ "Example", "md_doc_2integrations_2events.html#example", [
        [ "Creating listeners components", "md_doc_2integrations_2events.html#creating-listeners-components", null ],
        [ "Sending an event", "md_doc_2integrations_2events.html#sending-an-event", null ],
        [ "Accessing an input state", "md_doc_2integrations_2events.html#accessing-an-input-state", null ]
      ] ]
    ] ],
    [ "SFML integration", "md_doc_2integrations_2sfml.html", [
      [ "Description", "md_doc_2integrations_2sfml.html#description-1", null ],
      [ "Usage", "md_doc_2integrations_2sfml.html#usage", null ]
    ] ],
    [ "User Actions integration", "md_doc_2integrations_2user__actions.html", [
      [ "Description", "md_doc_2integrations_2user__actions.html#description-2", null ],
      [ "Usage", "md_doc_2integrations_2user__actions.html#usage-1", [
        [ "Register your actions and bindings", "md_doc_2integrations_2user__actions.html#register-your-actions-and-bindings", null ],
        [ "Listen to your actions", "md_doc_2integrations_2user__actions.html#listen-to-your-actions", [
          [ "ActionListener", "md_doc_2integrations_2user__actions.html#actionlistener", null ],
          [ "ActionListeners", "md_doc_2integrations_2user__actions.html#actionlisteners", null ],
          [ "ActionIdListener", "md_doc_2integrations_2user__actions.html#actionidlistener", null ]
        ] ],
        [ "Using intermediate actions", "md_doc_2integrations_2user__actions.html#using-intermediate-actions", null ],
        [ "Saving and loading the bindings", "md_doc_2integrations_2user__actions.html#saving-and-loading-the-bindings", [
          [ "Saving", "md_doc_2integrations_2user__actions.html#saving", null ],
          [ "Loading", "md_doc_2integrations_2user__actions.html#loading", null ]
        ] ]
      ] ]
    ] ],
    [ "Namespaces", "namespaces.html", [
      [ "Namespace List", "namespaces.html", "namespaces_dup" ],
      [ "Namespace Members", "namespacemembers.html", [
        [ "All", "namespacemembers.html", null ],
        [ "Functions", "namespacemembers_func.html", null ],
        [ "Variables", "namespacemembers_vars.html", null ],
        [ "Typedefs", "namespacemembers_type.html", null ]
      ] ]
    ] ],
    [ "Concepts", "concepts.html", "concepts" ],
    [ "Classes", "annotated.html", [
      [ "Class List", "annotated.html", "annotated_dup" ],
      [ "Class Index", "classes.html", null ],
      [ "Class Hierarchy", "hierarchy.html", "hierarchy" ],
      [ "Class Members", "functions.html", [
        [ "All", "functions.html", "functions_dup" ],
        [ "Functions", "functions_func.html", "functions_func" ],
        [ "Variables", "functions_vars.html", null ],
        [ "Typedefs", "functions_type.html", null ],
        [ "Enumerations", "functions_enum.html", null ],
        [ "Related Symbols", "functions_rela.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "File Members", "globals.html", [
        [ "All", "globals.html", null ],
        [ "Functions", "globals_func.html", null ],
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"01__basic__window_2main_8cpp.html",
"_shared_recursive_mutex_8hpp.html",
"classecstasy_1_1_instances.html#a60a0a52241c58dd76d10144fba5c8307",
"classecstasy_1_1_registry_1_1_registry_stack_query_memory_3_01util_1_1meta_1_1_traits_3_01_selecfcfaca0c668d0d83b5ea7096197c8168.html#ae6a0979483eb1a06e84922357f00ca28",
"classecstasy_1_1integration_1_1event_1_1_gamepad_combination_listener.html#a7908e765d57dfe6304ad121c839f539a",
"classecstasy_1_1integration_1_1event_1_1_keyboard.html#af7a1ffe443594240aaf7d589a7558fe3a69691c7bdcc3ce6d5d8a1361f22d04ac",
"classecstasy_1_1integration_1_1user__action_1_1_action_bindings.html#a4705e1be27e7b5d7543ca5d473b8f0a2",
"classecstasy_1_1query_1_1modifier_1_1_not.html#a15ec501fbb8ec49c4e6df219285fb4f2",
"classecstasy_1_1serialization_1_1_json_serializer.html#ae84160a3d01aa403043460835e9fa0b7",
"classutil_1_1_bit_set.html#adbc690e56987545c1ffef321f4bfc8cf",
"classutil_1_1serialization_1_1_i_node.html#ac05418b34fe552da8863de6063bbbe0b",
"classutil_1_1serialization_1_1_toml_node_factory.html#ab9e4ba48a48d2439769493556cfc76a6",
"cpp/algorithm/ranges/stable_partition.html",
"cpp/experimental/type_trait_variable_templates.html",
"cpp/memory/make_obj_using_allocator.html",
"cpp/string/byte/islower.html",
"cpp/utility/program/_Exit.html",
"group___r_a_p_i_d_j_s_o_n___e_r_r_o_r_s.html#gga7d3acf640886b1f2552dc8c4cd6dea60a2a81a684f39fc882ec99f07e86343f73",
"namespacerapidjson.html#a81379eb4e94a0386d71d15fda882ebc9a02748971d6ea0c5e22c69b8aa7f344ea",
"structecstasy_1_1integration_1_1event_1_1_event.html#a428f3f906bf2f40cf2dcec224662719aa7584c169de594c27f00084b93186fd7c",
"structecstasy_1_1query_1_1_select_1_1_filter_queryables.html#a7051c608bc6c336292ace020d23bd8c9"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';