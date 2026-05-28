importScripts('../game.js'); 

let wasmModule;

createGameModule({
    locateFile: function(path) {
        // ALWAYS look at the absolute root of the server, completely 
        // bypassing relative directory confusion.
        return '/' + path; 
    }
}).then(module => {
    wasmModule = module;
    postMessage({ type: 'READY' });
});

self.onmessage = function(e) {
    if (!wasmModule) return; 

    if (e.data.type === 'INIT_IDA') {
        const { name, width, height, patterns } = e.data.config;
        
        const cppPatterns = new wasmModule.VectorVectorInt();
        for (let row of patterns) {
            const cppRow = new wasmModule.VectorInt();
            for (let val of row) cppRow.push_back(val);
            cppPatterns.push_back(cppRow);
            cppRow.delete(); 
        }

        wasmModule.initIda(name, width, height, cppPatterns);
        cppPatterns.delete();
        postMessage({ type: 'INIT_DONE' });

    } else if (e.data.type === 'SOLVE') {
        const { gridArray, width, height } = e.data;
        
        const cppGrid = new wasmModule.VectorInt();
        for (let val of gridArray) {
            cppGrid.push_back(val);
        }
        
        try {
            const cppSolution = wasmModule.runIda(cppGrid, width, height);
            
            const solutionMoves = [];
            for (let i = 0; i < cppSolution.size(); i++) {
                solutionMoves.push(cppSolution.get(i));
            }
            cppSolution.delete(); 
            
            postMessage({ type: 'SOLUTION', solutionMoves });
        } catch (err) {
            postMessage({ type: 'ERROR', error: err.message });
        } finally {
            cppGrid.delete();
        }
    }
};