<script>
  import "./style.css";
  let code = ``;
  let tokens = [];
  let errors = [];
  let ast = null;
  let currentView = "kanban";

  async function analyze() {
    try {
      const response = await fetch("http://127.0.0.1:18080/analyze", {
        method: "POST",
        headers: { "Content-Type": "text/plain" },
        body: code,
      });

      const data = await response.json();

      if (response.ok) {
        ast = data;
        tokens = [
          {
            id: 1,
            lexema: "MISSION",
            tipo: "Palabra Reservada",
            linea: 1,
            col: 1,
          },
        ];
        errors = [];
        currentView = "kanban";
      } else {
        errors = [
          {
            id: 1,
            lexema: "Error",
            tipo: "Sintáctico",
            desc: data.error,
            linea: 0,
            col: 0,
          },
        ];
        currentView = "errors";
      }
    } catch (err) {
      console.error(err); // Esto imprimirá el error real en la consola de F12
      alert("Error en el proceso: " + err.message);
    }
  }

  function setView(view) {
    currentView = view;
  }
</script>

<main>
  <!-- Toolbar superior estilo Material Design 2016 -->
  <header class="app-bar">
    <div class="app-bar-inner">
      <div class="app-bar-brand">
        <span class="brand-icon">&#9670;</span>
        <div>
          <h1 class="app-title">Script</h1>
        </div>
      </div>
      <button class="fab" on:click={analyze} title="Analizar código">
        &#9654;
      </button>
    </div>
  </header>

  <!-- Layout principal responsivo -->
  <div class="main-layout">
    <!-- Panel editor -->
    <section class="card editor-card">
      <div class="card-header">
        <span class="card-header-dot dot-green"></span>
        <span class="card-header-dot dot-yellow"></span>
        <span class="card-header-dot dot-red"></span>
        <span class="card-label">editor.halo</span>
      </div>
      <textarea bind:value={code} spellcheck="false" class="code-editor"
      ></textarea>
      <div class="editor-footer">
        <span class="line-count">Script</span>
        <button class="btn-run" on:click={analyze}>&#9654; ANALIZAR</button>
      </div>
    </section>

    <!-- Panel resultados -->
    <section class="card results-card">
      <!-- Tabs estilo Material Design -->
      <nav class="tab-bar">
        <button
          class="tab-btn"
          class:tab-active={currentView === "kanban"}
          on:click={() => setView("kanban")}
        >
          <span class="tab-icon">&#9783;</span>
          <span class="tab-label">Tablero</span>
        </button>
        <button
          class="tab-btn"
          class:tab-active={currentView === "tokens"}
          on:click={() => setView("tokens")}
        >
          <span class="tab-icon">&#9670;</span>
          <span class="tab-label">Tokens</span>
        </button>
        <button
          class="tab-btn"
          class:tab-active={currentView === "errors"}
          on:click={() => setView("errors")}
        >
          <span class="tab-icon">&#9888;</span>
          <span class="tab-label">Errores</span>
        </button>
        <button
          class="tab-btn"
          class:tab-active={currentView === "dot"}
          on:click={() => setView("dot")}
        >
          <span class="tab-icon">&#9672;</span>
          <span class="tab-label">Graphviz</span>
        </button>
      </nav>

      <!-- Indicador de tab activo -->
      <div
        class="tab-ink-bar"
        style="left: {['kanban', 'tokens', 'errors', 'dot'].indexOf(
          currentView,
        ) * 25}%"
      ></div>

      <div class="panel-content">
        {#if currentView === "kanban"}
          <div class="kanban-board">
            {#if ast}
              <div class="kanban-column">
                <div class="column-header">
                  <span class="column-title">{ast.mision}</span>
                  <span class="column-chip"
                    >{ast.objetivos?.length ?? 0} objetivos</span
                  >
                </div>
                {#each ast.objetivos as obj}
                  <div class="kanban-card">
                    <p class="kanban-card-title">{obj.descripcion}</p>
                    <div class="kanban-card-footer">
                      <span class="xp-badge">
                        <span class="xp-icon">&#9733;</span>
                        {obj.recompensa} XP
                      </span>
                    </div>
                  </div>
                {/each}
              </div>
            {:else}
              <div class="empty-state">
                <span class="empty-icon">&#9674;</span>
                <p>Ejecuta el análisis para ver resultados</p>
              </div>
            {/if}
          </div>
        {:else if currentView === "tokens"}
          <div class="table-wrapper">
            <table class="data-table">
              <thead>
                <tr>
                  <th>#</th>
                  <th>Lexema</th>
                  <th>Tipo</th>
                  <th>Línea</th>
                </tr>
              </thead>
              <tbody>
                {#each tokens as t}
                  <tr>
                    <td class="muted">{t.id}</td>
                    <td class="accent">{t.lexema}</td>
                    <td>{t.tipo}</td>
                    <td class="muted">{t.linea}</td>
                  </tr>
                {/each}
                {#if tokens.length === 0}
                  <tr
                    ><td colspan="4" class="empty-row"
                      >Sin tokens. Analiza primero.</td
                    ></tr
                  >
                {/if}
              </tbody>
            </table>
          </div>
        {:else if currentView === "errors"}
          <div class="error-list">
            {#each errors as e}
              <div class="error-card">
                <div class="error-card-header">
                  <span class="error-type-badge">{e.tipo}</span>
                  <span class="error-location">Fila {e.linea}, Col {e.col}</span
                  >
                </div>
                <p class="error-desc">{e.desc}</p>
              </div>
            {/each}
            {#if errors.length === 0}
              <div class="empty-state success">
                <span class="empty-icon">&#10003;</span>
                <p>Sin errores detectados</p>
              </div>
            {/if}
          </div>
        {:else if currentView === "dot"}
          <div class="dot-panel">
            <div class="dot-label">Graphviz DOT Output</div>
            <pre class="dot-code">digraph G {"{"}
  rankdir=TB;
  node [shape=box, style=filled, fillcolor="#0d1f2d", color="#00bfff", fontcolor="#00bfff"];
  edge [color="#32cd32"];

  "{ast?.mision || "Misión"}" -> "Objetivos";
  "Objetivos" -> "Completar";
{"}"}</pre>
          </div>
        {/if}
      </div>
    </section>
  </div>

  <!-- Status bar inferior -->
  <footer class="status-bar">
    <span class="status-item status-center">Script Compiler</span>
  </footer>
</main>
