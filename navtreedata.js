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
    [ "ECSTASY (Entity Component System Toward Architecture Saving Years)", "index.html", null ],
    [ "Building", "md_doc_2_building.html", null ],
    [ "Glossary", "md_doc_2_glossary.html", [
      [ "Entity", "md_doc_2_glossary.html#autotoc_md15", null ],
      [ "Component", "md_doc_2_glossary.html#autotoc_md16", null ],
      [ "Storage", "md_doc_2_glossary.html#autotoc_md17", null ],
      [ "System", "md_doc_2_glossary.html#autotoc_md18", null ],
      [ "Resource", "md_doc_2_glossary.html#autotoc_md19", null ],
      [ "Registry", "md_doc_2_glossary.html#autotoc_md20", null ],
      [ "Query", "md_doc_2_glossary.html#autotoc_md21", [
        [ "Queryable", "md_doc_2_glossary.html#autotoc_md22", null ],
        [ "Batch Query", "md_doc_2_glossary.html#autotoc_md23", null ],
        [ "Modifier", "md_doc_2_glossary.html#autotoc_md24", null ],
        [ "Condition", "md_doc_2_glossary.html#autotoc_md25", null ]
      ] ],
      [ "Thread", "md_doc_2_glossary.html#autotoc_md26", [
        [ "Lockable", "md_doc_2_glossary.html#autotoc_md27", null ]
      ] ],
      [ "Serialization", "md_doc_2_glossary.html#autotoc_md28", [
        [ "Serializer actions", "md_doc_2_glossary.html#autotoc_md29", [
          [ "Export", "md_doc_2_glossary.html#autotoc_md30", null ],
          [ "Import", "md_doc_2_glossary.html#autotoc_md31", null ]
        ] ],
        [ "Variable actions", "md_doc_2_glossary.html#autotoc_md32", [
          [ "Serialize: Save", "md_doc_2_glossary.html#autotoc_md33", null ],
          [ "Deserialize: Update", "md_doc_2_glossary.html#autotoc_md34", null ],
          [ "Deserialize: Load", "md_doc_2_glossary.html#autotoc_md35", null ]
        ] ]
      ] ]
    ] ],
    [ "Tutorial", "md_doc_2_tutorial.html", [
      [ "Getting Started", "md_doc_2_tutorial.html#GettingStarted", null ],
      [ "Using entities", "md_doc_2_tutorial.html#autotoc_md37", [
        [ "Creating entities", "md_doc_2_tutorial.html#autotoc_md38", [
          [ "Using type resolution", "md_doc_2_tutorial.html#autotoc_md39", null ]
        ] ],
        [ "Manage entity component", "md_doc_2_tutorial.html#autotoc_md40", null ],
        [ "Delete entities", "md_doc_2_tutorial.html#autotoc_md41", null ]
      ] ],
      [ "Implement a custom storage", "md_doc_2_tutorial.html#CustomStorageTutorial", null ],
      [ "Making registry queries", "md_doc_2_tutorial.html#QueryTutorial", [
        [ "Select ... Where ...", "md_doc_2_tutorial.html#SelectWhereTutorial", null ],
        [ "Using modifiers", "md_doc_2_tutorial.html#ModifiersTutorial", null ],
        [ "Using conditions", "md_doc_2_tutorial.html#ConditionsTutorial", null ]
      ] ],
      [ "Using systems", "md_doc_2_tutorial.html#autotoc_md42", null ],
      [ "Ensuring Thread Safety", "md_doc_2_tutorial.html#EnsuringThreadSafety", null ],
      [ "Serializing your entities", "md_doc_2_tutorial.html#autotoc_md43", [
        [ "Using a serializer", "md_doc_2_tutorial.html#autotoc_md44", [
          [ "Common types", "md_doc_2_tutorial.html#autotoc_md45", null ],
          [ "Custom types", "md_doc_2_tutorial.html#autotoc_md46", null ],
          [ "Working with Entities", "md_doc_2_tutorial.html#autotoc_md47", null ]
        ] ]
      ] ]
    ] ],
    [ "Technical Implementations", "md_doc_2_technical_implementation.html", [
      [ "Entities Management", "md_doc_2_technical_implementation.html#autotoc_md49", null ],
      [ "Components Management (MapStorage)", "md_doc_2_technical_implementation.html#autotoc_md50", null ],
      [ "Query", "md_doc_2_technical_implementation.html#autotoc_md51", null ]
    ] ],
    [ "Events integration", "md_doc_2integrations_2events.html", [
      [ "Description", "md_doc_2integrations_2events.html#autotoc_md60", [
        [ "How to get feedback", "md_doc_2integrations_2events.html#autotoc_md61", null ],
        [ "Event flow", "md_doc_2integrations_2events.html#autotoc_md62", null ]
      ] ],
      [ "Example", "md_doc_2integrations_2events.html#autotoc_md63", [
        [ "Creating listeners components", "md_doc_2integrations_2events.html#autotoc_md64", null ],
        [ "Sending an event", "md_doc_2integrations_2events.html#autotoc_md65", null ],
        [ "Accessing an input state", "md_doc_2integrations_2events.html#autotoc_md66", null ]
      ] ]
    ] ],
    [ "SFML integration", "md_doc_2integrations_2sfml.html", [
      [ "Description", "md_doc_2integrations_2sfml.html#autotoc_md68", null ],
      [ "Usage", "md_doc_2integrations_2sfml.html#autotoc_md69", null ]
    ] ],
    [ "User Actions integration", "md_doc_2integrations_2user__actions.html", [
      [ "Description", "md_doc_2integrations_2user__actions.html#autotoc_md71", null ],
      [ "Usage", "md_doc_2integrations_2user__actions.html#autotoc_md72", [
        [ "Register your actions and bindings", "md_doc_2integrations_2user__actions.html#autotoc_md73", null ],
        [ "Listen to your actions", "md_doc_2integrations_2user__actions.html#autotoc_md74", [
          [ "ActionListener", "md_doc_2integrations_2user__actions.html#autotoc_md75", null ],
          [ "ActionListeners", "md_doc_2integrations_2user__actions.html#autotoc_md76", null ],
          [ "ActionIdListener", "md_doc_2integrations_2user__actions.html#autotoc_md77", null ]
        ] ],
        [ "Using intermediate actions", "md_doc_2integrations_2user__actions.html#autotoc_md78", null ],
        [ "Saving and loading the bindings", "md_doc_2integrations_2user__actions.html#autotoc_md79", [
          [ "Saving", "md_doc_2integrations_2user__actions.html#autotoc_md80", null ],
          [ "Loading", "md_doc_2integrations_2user__actions.html#autotoc_md81", null ]
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
"_storage_concepts_8hpp.html",
"classecstasy_1_1_map_storage.html#a0fafe82f65b037356382974075ff258b",
"classecstasy_1_1_registry_entity.html#a64b5b5609880025e9d195014b847bae0",
"classecstasy_1_1integration_1_1event_1_1_gamepad_combination_listener.html#a70ce17f6370447ace361f856e06231da",
"classecstasy_1_1integration_1_1sfml_1_1_render_window.html#ac7c1ea853334f98e6d6827ad3645ebf5",
"classecstasy_1_1query_1_1modifier_1_1_binary_modifier.html#a88285a49ce3fa1696cd254d45bdaa63e",
"classecstasy_1_1serialization_1_1_raw_serializer.html#a9b899be4e342300b605a806e28670bcb",
"classutil_1_1_stack_allocator.html#a083d90b56e98ae177f3edf206ceea3fb",
"classutil_1_1serialization_1_1_i_node_factory.html#ae1140c0f8dfeacf7bbf88d2befa0c7cf",
"classutil_1_1serialization_1_1_toml_object_node.html#abccc09a7daf3291447205ef54ea5c479",
"cpp/algorithm/swap_ranges.html",
"cpp/experimental/unordered_multimap/erase_if.html",
"cpp/memory/uninitialized_default_construct_n.html",
"cpp/string/byte/strtoul.html",
"dir_30970fbe9de3918f98e58b7106d748f8.html",
"md_doc_2integrations_2user__actions.html#autotoc_md76",
"structecstasy_1_1integration_1_1event_1_1_event.html#a428f3f906bf2f40cf2dcec224662719aac17e6984c1e5a887a49831beadee6bca",
"structecstasy_1_1query_1_1_select_1_1_sortered_tie.html#a899b147f0c90051c95c2c6584ac716aa"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';